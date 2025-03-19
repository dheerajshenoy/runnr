#include "GameScene.hpp"
#include <bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include <raylib.h>


GameScene::GameScene()
    : Scene()
{
    initShader();
    initCamera();
    initPhysics();
    initPlayer();
    spawnPlatform();
}

GameScene::~GameScene()
{
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray().at(i);
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    for (int i=0; i < m_platform_list.size(); i++)
    {
        Platform* p = m_platform_list.at(i);
        delete p;
        p = nullptr;
    }

    // Clean Player
    delete m_player;

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
    m_platform_list.clear();

    UnloadShader(m_shadowShader);
    UnloadShadowmapRenderTexture(m_shadowMapTexture);
}

void GameScene::update(const float &dt)
{
    m_dynamicsWorld->stepSimulation(dt, 10);

    if (m_player->isOnGround()) {
        m_player->is_on_ground = true;
    }

    Vector3 cameraPos = m_camera.position;
    SetShaderValue(m_shadowShader, m_shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);

    m_lightDir = Vector3Normalize(m_lightDir);
    m_lightCam.position = Vector3Scale(m_lightDir, -15.0f);
    SetShaderValue(m_shadowShader, m_lightDirLoc, &m_lightDir, SHADER_UNIFORM_VEC3);

    cameraFollowPlayer();

    updatePlatforms(dt);

    handleInput();
}

void GameScene::render()
{
    // shaderRun(); // TODO: Shadow mapping

    UpdateCamera(&m_camera, CAMERA_CUSTOM);
    ClearBackground(BLACK);
    BeginMode3D(m_camera);

    DrawScene();

    EndMode3D();

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20.0, BLACK);

    EndDrawing();
}

void GameScene::initPhysics() noexcept
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -m_gravity, 0));
}

void GameScene::updatePhysics() noexcept
{}

void GameScene::initCamera() noexcept
{
    m_camera.position = Vector3 { 0, 2.0, 10.0 };
    m_camera.target = Vector3 { 0, 0.0, +0.0 };
    m_camera.up = { 0, 1.0, 0 };
    m_camera.fovy = 90.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void GameScene::renderSystem() noexcept
{
    // draw player
    m_player->render();

    // draw platforms
    for (auto &platform : m_platform_list)
        platform->render();
}

void GameScene::handleInput() noexcept
{
    if (IsKeyDown(KEY_A)) {
        m_player->move(Player::MoveDirection::LEFT);
    }

    if (IsKeyDown(KEY_D)) {
        m_player->move(Player::MoveDirection::RIGHT);
    }

    if (IsKeyDown(KEY_SPACE)) {
        m_player->jump();
    }
}

void GameScene::initPlayer() noexcept
{
    m_player = new Player(btVector3(0, 10.0f, 0), m_dynamicsWorld);
    m_player->model.materials[0].shader = m_shadowShader;
    m_dynamicsWorld->addRigidBody(m_player->body);
}

void GameScene::spawnPlatform() noexcept
{
    float randomX = GetRandomValue(-10, 10);
    float length = 50.0f;
    float gap = GetRandomValue(5, 10);
    float nextZ = lastSpawnZ - gap - length;
    printf("%f", lastSpawnZ);

    btVector3 pos(randomX, -5.0f, nextZ);
    btVector3 size(10.0f, 1.0f, length);
    Platform *platform = new Platform(pos, size);
    platform->model.materials[0].shader = m_shadowShader;
    m_dynamicsWorld->addRigidBody(platform->body);
    m_platform_list.push_back(platform);
}

void GameScene::initShader() noexcept
{
    m_shadowShader = LoadShader("../src/shaders/shadowmap.vs", "../src/shaders/shadowmap.fs");
    m_shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_shadowShader, "viewPos");
    Vector4 lightColorNormalized = ColorNormalize(m_lightColor);
    m_lightDirLoc = GetShaderLocation(m_shadowShader, "lightDir");
    int lightColLoc = GetShaderLocation(m_shadowShader, "lightColor");
    SetShaderValue(m_shadowShader, m_lightDirLoc, &m_lightDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(m_shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    int ambientLoc = GetShaderLocation(m_shadowShader, "ambient");
    SetShaderValue(m_shadowShader, ambientLoc, m_ambient, SHADER_UNIFORM_VEC4);
    m_lightVPLoc = GetShaderLocation(m_shadowShader, "lightVP");
    m_shadowMapLoc = GetShaderLocation(m_shadowShader, "shadowMap");
    SetShaderValue(m_shadowShader,
                   GetShaderLocation(m_shadowShader, "shadowMapResolution"),
                   &m_shadowMapResolution,
                   SHADER_UNIFORM_INT);

    RenderTexture2D shadowMap = LoadShadowmapRenderTexture(m_shadowMapResolution, m_shadowMapResolution);
    m_lightCam.position = Vector3Scale(m_lightDir, -15.0f);
    m_lightCam.target = Vector3Zero();
    // Use an orthographic projection for directional lights
    m_lightCam.projection = CAMERA_ORTHOGRAPHIC;
    m_lightCam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    m_lightCam.fovy = 20.0f;
}

void GameScene::shaderRun() noexcept
{
    // First, render all objects into the shadowmap
    // The idea is, we record all the objects' depths (as rendered from the light source's point of view) in a buffer
    // Anything that is "visible" to the light is in light, anything that isn't is in shadow
    // We can later use the depth buffer when rendering everything from the player's point of view
    // to determine whether a given point is "visible" to the light
    SetShaderValue(m_shadowShader, m_shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &m_camera.position, SHADER_UNIFORM_VEC3);
    BeginDrawing();
    BeginTextureMode(m_shadowMapTexture);
    ClearBackground(WHITE);
    BeginMode3D(m_lightCam);
    m_lightView = rlGetMatrixModelview();
    m_lightProj = rlGetMatrixProjection();
    DrawScene();
    EndMode3D();
    EndTextureMode();
    Matrix lightViewProj = MatrixMultiply(m_lightView, m_lightProj);

    ClearBackground(RAYWHITE);

    SetShaderValueMatrix(m_shadowShader, m_lightVPLoc, lightViewProj);
    rlEnableShader(m_shadowShader.id);

    int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
    rlActiveTextureSlot(10);
    rlEnableTexture(m_shadowMapTexture.depth.id);
    rlSetUniform(m_shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);
}

RenderTexture2D GameScene::LoadShadowmapRenderTexture(const int &width,
                                                      const int &height) noexcept
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
void GameScene::UnloadShadowmapRenderTexture(RenderTexture2D target) noexcept
{
    if (target.id > 0)
    {
        // NOTE: Depth texture/renderbuffer is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}

void GameScene::DrawScene() noexcept
{
    renderSystem();
}

void GameScene::cameraFollowPlayer() noexcept
{
    auto &pos = m_player->transform.getOrigin();
    m_camera.position.y = pos.getY() + 10.0f;
    m_camera.position.z = pos.getZ() + 10.0f;
}

void GameScene::updatePlatforms(const float &dt) noexcept
{

    for (auto &platform : m_platform_list)
    {
        platform->update(dt);
    }

    // Spawn new platforms when the last platform is halfway off the screen
    if (!m_platform_list.empty()) {
        Platform* lastPlatform = m_platform_list.back();
        float lastPlatformZ = lastPlatform->body->getWorldTransform().getOrigin().getZ();
        if (lastPlatformZ > 10.0f) {
            lastSpawnZ = lastPlatformZ;
            spawnPlatform();
        }
    }

    m_platform_list.erase(std::remove_if(m_platform_list.begin(),
                                         m_platform_list.end(),
                                         [this](Platform *platform) {
                                             if (platform->isOffScreen())
                                             {
                                                 deletePlatform(platform);
                                                 return true;
                                             }
                                             return false;
                                         }), m_platform_list.end());



}

void GameScene::deletePlatform(Platform *platform) noexcept
{
    if (!platform)
        return;

    platform->cleanup(m_dynamicsWorld);
}
