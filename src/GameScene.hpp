#pragma once

#include "Scene.hpp"
#include "entt/entt.hpp"
#include "SceneManager.hpp"
#include "Player.hpp"
#include <raylib.h>
#include <raymath.h>
#include <bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include "Platform.hpp"
#include <rlgl.h>
#include <algorithm>
#include "Box.hpp"

class GameScene : public Scene {

    public:
    GameScene();
    ~GameScene();

    void update(const float &dt);
    void render();

    enum class PlatformType {
        REGULAR = 0,
    };

    private:
    Player *m_player { nullptr };
    float m_gravity = 10.0f;
    void handleInput() noexcept;
    void renderSystem() noexcept;
    void initCamera() noexcept;
    void initPlayer() noexcept;
    void initPhysics() noexcept;
    void initShader() noexcept;
    void updatePhysics() noexcept;
    void spawnPlatform() noexcept;
    entt::registry m_registry;
    Camera3D m_camera { 0 };
    Camera3D m_lightCam { 0 };
    int m_lightDirLoc;

    Vector3 m_lightDir { Vector3Normalize((Vector3){ 0.35f, -1.0f, -0.35f }) };
    Color m_lightColor { WHITE };
    float m_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    Shader m_shadowShader;

    btDefaultCollisionConfiguration *m_collisionConfiguration { nullptr };
    btCollisionDispatcher *m_dispatcher { nullptr };
    btBroadphaseInterface *m_overlappingPairCache { nullptr };
    btSequentialImpulseConstraintSolver *m_solver { nullptr };
    btDiscreteDynamicsWorld *m_dynamicsWorld { nullptr };
    std::vector<Platform*> m_platform_list;

    int m_lightVPLoc;
    int m_shadowMapLoc;
    int m_shadowMapResolution = 2048; // Higher size = more accurate shadows
    RenderTexture m_shadowMapTexture;
    void shaderRun() noexcept;
    Matrix m_lightView;
    Matrix m_lightProj;
    RenderTexture2D LoadShadowmapRenderTexture(const int &width,
                                               const int &height) noexcept;
    void UnloadShadowmapRenderTexture(RenderTexture2D target) noexcept;
    void DrawScene() noexcept;
    void cameraFollowPlayer() noexcept;
    void updatePlatforms(const float &dt) noexcept;

    void deletePlatform(Platform *platform) noexcept;

    float lastSpawnZ { 50.0f };

    Box *box { nullptr };
};
