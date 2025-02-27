#include "GameScene.hpp"
#include <raylib.h>

GameScene::GameScene()
:   m_game_over(false),
    m_info_overlay(false),
    m_score(0)
{
    initCamera();
    m_player = new Player();
    m_player->setPos({0, 10, 0});
    Floor *static_floor = new Floor();
    static_floor->setSpeed(4);
    static_floor->setPos({0, 0, 0});
    static_floor->setSize({20, 1, 10});
    m_floor_list.push_back(static_floor);

    m_jump_sound = LoadSound("../resources/sounds/Coffee1.mp3");
    m_resume_btn = new Button("Resume", 10, 40, 100.0, 50.0);
    m_exit_btn = new Button("Exit", 10, 100, 100.0, 50.0);
    m_resume_btn->pressed([&]() {
        m_paused = !m_paused;
    });
    m_exit_btn->pressed([]() {
        Runnr::GetInstance().exit();
        // TODO: Segfault
    });

    initFloorSpawning();
    initShader();
}

GameScene::~GameScene() {
    delete m_player;
    delete m_resume_btn;
    delete m_exit_btn;
    UnloadShader(m_shadow_shader);
    UnloadSound(m_jump_sound);
    UnloadShadowmapRenderTexture(m_shadowMap);
    // UnloadSound(m_on_land_sound);
}

void GameScene::initShader() noexcept {
    m_shadow_shader = LoadShader("../src/shaders/shadowmap.vs",
                                 "../src/shaders/shadowmap.fs");

    m_shadow_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_shadow_shader, "viewPos");
    m_lightDir = Vector3Normalize({-5.0, -5.0, 10.0});
    auto lightColor = WHITE;
    Vector4 lightColorNormalized = ColorNormalize(lightColor);
    int lightDirLoc = GetShaderLocation(m_shadow_shader, "lightDir");
    int lightColLoc = GetShaderLocation(m_shadow_shader, "lightColor");

    SetShaderValue(m_shadow_shader, lightDirLoc, &m_lightDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(m_shadow_shader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    int ambientLoc = GetShaderLocation(m_shadow_shader, "ambient");
    float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(m_shadow_shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    m_lightVPLoc = GetShaderLocation(m_shadow_shader, "lightVP");
    m_shadowMapLoc = GetShaderLocation(m_shadow_shader, "shadowMap");
    int shadowMapResolution = 1024;
    SetShaderValue(m_shadow_shader, GetShaderLocation(m_shadow_shader, "shadowMapResolution"),
                   &shadowMapResolution, SHADER_UNIFORM_INT);
    m_shadowMap = LoadShadowmapRenderTexture(1024, 1024);
    // For the shadowmapping algorithm, we will be rendering everything from the light's point of view

    m_lightCam.position = Vector3Scale(m_lightDir, -15.0f);
    m_lightCam.target = Vector3Zero();
    // Use an orthographic projection for directional lights
    m_lightCam.projection = CAMERA_ORTHOGRAPHIC;
    m_lightCam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    m_lightCam.fovy = 20.0f;
    m_player->setShader(m_shadow_shader);
}

void GameScene::initCamera() noexcept {
    m_camera.position   = (Vector3){ 0.0f, 10.0f, 10.0f }; // Camera position
    m_camera.target     = (Vector3){ 0.0f, 4.0f, 5.0f }; // Camera looking at point
    m_camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
    m_camera.fovy       = 90.0f; // Camera field-of-view Y
    m_camera.projection = CAMERA_PERSPECTIVE; // Camera projection type
}

void GameScene::initFloorSpawning() noexcept {

    m_timer.setInterval(2);

    m_timer.setCallback([&]() {
        Floor *floor = new Floor();
        floor->setSpeed(10.0f);
        floor->setSize({20, 1, 5});
        floor->setColor({255, 0, 0});
        auto lastFloor = m_floor_list.back();
        auto lastFloorPos = lastFloor->pos();
        auto lastX = lastFloorPos.x;
        auto lastY = lastFloorPos.y;
        auto lastZ = lastFloorPos.z;
        int X = GetRandomValue(-10, 10);
        int Y = GetRandomValue(-1, 3);
        auto pos = Vector3 { X, Y, lastZ - lastFloor->size().z * 0.5 - 20};
        floor->setPos(pos);
        floor->setShader(m_shadow_shader);
        m_floor_list.push_back(floor);
    });

    m_timer.start();

}

void GameScene::update(const float &dt) {
    handleInput();
    if (!m_paused) {
        m_floor_list.erase(std::remove_if(m_floor_list.begin(), m_floor_list.end(),
                                          [](Floor* b) { return b->is_off_screen(); }),
                           m_floor_list.end());

        for (const auto &floor : m_floor_list)
            floor->update(dt);

        m_player->update(dt, m_floor_list);

        m_timer.update(dt);

        if (m_player->pos().y <= -10) {
            SceneManager::GetInstance().changeScene(new GameOverScene(m_score));
        }

        SetShaderValue(m_shadow_shader, GetShaderLocation(m_shadow_shader, "lightDir"),
                       &m_lightDir, SHADER_UNIFORM_VEC3);

        m_score += 1;

    } else {
        m_exit_btn->update();
        m_resume_btn->update();
    }
}

void GameScene::render() {

    BeginDrawing();

    Matrix lightView;
    Matrix lightProj;
    BeginTextureMode(m_shadowMap);
    ClearBackground(BLANK);
    UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    BeginMode3D(m_lightCam);

    lightView = rlGetMatrixModelview();
    lightProj = rlGetMatrixProjection();

    for(const auto &floor : m_floor_list)
        floor->render();

    m_player->render();
    EndMode3D();
    EndTextureMode();

    Matrix lightViewProj = MatrixMultiply(lightView, lightProj);

    ClearBackground(RAYWHITE);

    SetShaderValueMatrix(m_shadow_shader, m_lightVPLoc, lightViewProj);

    rlEnableShader(m_shadow_shader.id);
    int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
    rlActiveTextureSlot(10);
    rlEnableTexture(m_shadowMap.depth.id);
    rlSetUniform(m_shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

    BeginMode3D(m_camera);

        for(const auto &floor : m_floor_list)
            floor->render();

        m_player->render();

    EndMode3D();

    if (m_info_overlay)
        renderInfo();

    DrawText(TextFormat("Score: %d", m_score), 10, 10, 20, RAYWHITE);

    if (m_paused)
        renderPausedMenu();

    EndDrawing();
}

void GameScene::handleInput() noexcept {
    if (IsKeyPressed(KEY_F3)) {
        m_info_overlay = !m_info_overlay;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        m_paused = !m_paused;
    }
}

void GameScene::renderInfo() noexcept {
    auto player_pos = m_player->pos();
    DrawText(TextFormat("Position = x: %.2f, y: %.2f, z: %.2f", player_pos.x, player_pos.y, player_pos.z), 10, 10, 20, RAYWHITE);
}

void GameScene::renderPausedMenu() noexcept {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
    DrawText("Paused", 10, 10, 20, RAYWHITE);
    m_resume_btn->render();
    m_exit_btn->render();
}

RenderTexture2D GameScene::LoadShadowmapRenderTexture(const int &width, const int &height) noexcept
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // We don't need a color texture for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

// Unload shadowmap render texture from GPU memory (VRAM)
void GameScene::UnloadShadowmapRenderTexture(const RenderTexture2D &target) noexcept
{
    if (target.id > 0)
    {
        // NOTE: Depth texture/renderbuffer is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}
