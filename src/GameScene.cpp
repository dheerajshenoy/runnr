#include "GameScene.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "PlayerCollisionCallback.hpp"
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <raylib.h>

GameScene::GameScene()
    : Scene()
{
    initShader();
    initCamera();
    initPhysics();
    initPlayer();
    spawnPlatform();
    m_seed = time(nullptr);
    SetRandomSeed(m_seed);

    m_player->body->setCcdMotionThreshold(1e-7);
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

    delete m_player;
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;

    UnloadShader(m_shadowShader);
    UnloadShadowmapRenderTexture(m_shadowMapTexture);
}

void GameScene::update(const float &dt)
{

    PlayerCollisionCallback playerCollisionCallback(m_player);
    m_dynamicsWorld->contactTest(m_player->body, playerCollisionCallback);
    UpdateCamera(&m_camera, CAMERA_CUSTOM);
    m_dynamicsWorld->stepSimulation(dt, 10);

    if (!m_player->is_on_ground) {
        if (m_player->isPlayerOnGround()) {
            m_player->is_on_ground = true;
        }
    }

    Vector3 cameraPos = m_camera.position;
    SetShaderValue(m_shadowShader, m_shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);

    m_lightDir = Vector3Normalize(m_lightDir);
    m_lightCam.position = Vector3Scale(m_lightDir, -15.0f);
    SetShaderValue(m_shadowShader, m_lightDirLoc, &m_lightDir, SHADER_UNIFORM_VEC3);

    cameraFollowPlayer();
    handleInput();

    updatePlatforms(dt);
    cleanupPlatforms();
}

void GameScene::render()
{
    // shaderRun(); // TODO: Shadow mapping
    // m_player->model.materials[0].shader = m_shadowShader;
    BeginDrawing();
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
    auto view = GetAllEntitiesWith<PlatformComponent>();
    btTransform transform;
    for (const auto &e : view)
    {
        Entity entity { e, this };
        auto &render = entity.GetComponent<RenderComponent>();
        auto &rb = entity.GetComponent<RigidBodyComponent>();
        auto &size = render.dimension.size;
        rb.body->getMotionState()->getWorldTransform(transform);
        auto &pos = transform.getOrigin();

        DrawModel(render.model,
                  (Vector3) { pos.getX(), pos.getY(), pos.getZ() },
                  1.0f,
                  RED);
    }

    // draw powerups
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
    m_player = new Player(btVector3(0, 5.0f, 0), m_dynamicsWorld);
    m_player->model.materials[0].shader = m_shadowShader;
    m_dynamicsWorld->addRigidBody(m_player->body);
}

void GameScene::spawnPlatform() noexcept
{
    float randomX = GetRandomValue(-10, 10);
    m_length = GetRandomValue(20, 40);
    float gap = GetRandomValue(0, 10);
    float nextZ = lastSpawnZ - gap - m_length;
    float angle = GetRandomValue(2, 10);

    btVector3 pos(randomX, -5.0f, nextZ);
    btVector3 size(10.0f, 1.0f, m_length);

    lastSpawnZ = nextZ;
    CreatePlatform(pos, size);
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
    BeginDrawing();
    // First, render all objects into the shadowmap
    // The idea is, we record all the objects' depths (as rendered from the light source's point of view) in a buffer
    // Anything that is "visible" to the light is in light, anything that isn't is in shadow
    // We can later use the depth buffer when rendering everything from the player's point of view
    // to determine whether a given point is "visible" to the light
    SetShaderValue(m_shadowShader, m_shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &m_camera.position, SHADER_UNIFORM_VEC3);
    BeginTextureMode(m_shadowMapTexture);
    {
        ClearBackground(WHITE);
        BeginMode3D(m_lightCam);
        {
            m_lightView = rlGetMatrixModelview();
            m_lightProj = rlGetMatrixProjection();
            DrawScene();
        }
        EndMode3D();
    }
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

Entity GameScene::CreateEntity(const std::string &tag) noexcept
{
    Entity entity { registry.create(), this };
    entity.AddComponent<TagComponent>(tag);
    return entity;
}

void GameScene::DestroyEntity(Entity entity) noexcept
{
    if (entity.HasComponent<RenderComponent>())
    {
        auto& render = entity.GetComponent<RenderComponent>();
        UnloadModel(render.model);
    }
    registry.destroy(entity);
}

void GameScene::DestroyEntity(const entt::entity entity) noexcept
{
    registry.destroy(entity);
}

void GameScene::CreatePlatform(const btVector3 &pos,
                               const btVector3 &size) noexcept
{
    btTransform transform;
    auto platform = CreateEntity("Platform");
    lastPlatformHandle = platform.getHandle();

    platform.AddComponent<PlatformComponent>();

    auto &render = platform.AddComponent<RenderComponent>();
    auto tmp = size * 0.5;
    render.dimension.size = tmp;
    render.model = LoadModelFromMesh(GenMeshCube(size.getX(), size.getY(), size.getZ()));
    render.model.materials[0].shader = m_shadowShader;

    auto &rb = platform.AddComponent<RigidBodyComponent>();
    rb.shape = new btBoxShape(size * 0.5);

    transform.setIdentity();
    transform.setOrigin(pos);

    rb.motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, rb.motionState, rb.shape);
    rb.body = new btRigidBody(rbInfo);

    rb.body->setActivationState(DISABLE_DEACTIVATION);

    m_dynamicsWorld->addRigidBody(rb.body);
}

void GameScene::CreateRigidBody() noexcept
{

}

void GameScene::updatePlatforms(const float &dt) noexcept
{
    // Move platforms towards the player

    auto view = GetAllEntitiesWith<PlatformComponent, RigidBodyComponent>();
    for (const auto &platform : view)
    {
        auto *body = view.get<RigidBodyComponent>(platform).body;
        btTransform transform;
        auto *motionState = body->getMotionState();
        motionState->getWorldTransform(transform);
        auto &pos = transform.getOrigin();
        pos.setZ(pos.getZ() + 10.0f * dt);
        transform.setOrigin(pos);
        motionState->setWorldTransform(transform);
        body->setWorldTransform(transform);
    }

    // Check and Spawn new Platforms

    btTransform transform;
    auto &rb = view.get<RigidBodyComponent>(lastPlatformHandle);
    rb.body->getMotionState()->getWorldTransform(transform);

    float lastPlatformZ = transform.getOrigin().getZ();

    lastSpawnZ = lastPlatformZ;

    if (lastPlatformZ > )
    {
        spawnPlatform();
    }

}

void GameScene::cleanupPlatforms() noexcept
{
    btTransform transform;
    std::vector<Entity> entitiesToDelete;
    auto view = GetAllEntitiesWith<PlatformComponent, RigidBodyComponent>();
    for (auto platform : view)
    {
        Entity entity { platform, this };
        auto &rb = entity.GetComponent<RigidBodyComponent>();
        auto *body = rb.body;
        body->getMotionState()->getWorldTransform(transform);
        auto &pos = transform.getOrigin();

        // TODO: Cleanup platforms
        // if (pos.getZ() > 0.0f)
        // {
        //     m_dynamicsWorld->removeRigidBody(body);
        //     entitiesToDelete.push_back(entity);
        // }

    }

    for (auto &entity : entitiesToDelete)
        DestroyEntity(entity);
}
