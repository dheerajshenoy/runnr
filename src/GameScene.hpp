#pragma once

#include "Scene.hpp"

#include "entt/entt.hpp"
#include "SceneManager.hpp"
#include "Player.hpp"
#include <raylib.h>
#include <raymath.h>
#include "bullet.hpp"
#include "Platform.hpp"
#include <rlgl.h>
#include <algorithm>
#include "Box.hpp"
#include <time.h>
#include "Powerup.hpp"
#include "PlayerCollisionCallback.hpp"
#include "Entity.hpp"
#include "TimerManager.hpp"

class GameScene : public Scene {

    public:
    GameScene();
    ~GameScene();

    Player *player { nullptr };

    void update(const float &dt);
    void render();

    enum class PlatformType {
        REGULAR = 0,
    };

    Entity CreateEntity(const std::string &tag = std::string()) noexcept;
    void DestroyEntity(const Entity entity) noexcept;
    void DestroyEntity(const entt::entity entity) noexcept;

    void ApplyPowerupEffect(const PowerupComponent::PowerupType &type) noexcept;
    btDiscreteDynamicsWorld *dynamicsWorld { nullptr };

    private:
    float m_gravity = 10.0f;
    void handleInput() noexcept;
    void renderSystem() noexcept;
    void initCamera() noexcept;
    void initPlayer() noexcept;
    void initPhysics() noexcept;
    void initShader() noexcept;
    void updatePhysics() noexcept;
    void updatePlatforms(const float &dt) noexcept;
    void updatePowerups(const float &dt) noexcept;
    void cleanupPlatforms() noexcept;
    void spawnPlatform() noexcept;
    Camera3D m_camera { 0 };
    // 3d Collision Stuff
    btDefaultCollisionConfiguration *m_collisionConfiguration { nullptr };
    btCollisionDispatcher *m_dispatcher { nullptr };
    btBroadphaseInterface *m_overlappingPairCache { nullptr };
    btSequentialImpulseConstraintSolver *m_solver { nullptr };

    // Shader Stuff
    Camera3D m_lightCam { 0 };
    int m_lightDirLoc;

    Vector3 m_lightDir { Vector3Normalize((Vector3){ 0.35f, -1.0f, -0.35f }) };
    Color m_lightColor { WHITE };
    float m_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    Shader m_shadowShader;

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

    float lastSpawnZ { 50.0f };
    entt::entity lastPlatformHandle { entt::null };
    float m_length { 50.0f };
    float m_angle { 0.0f };

    time_t m_seed;

    Box *box { nullptr };

    Entity CreatePlatform(const btVector3 &pos,
                        const btVector3 &size,
                        const float &angle) noexcept;

    Entity CreatePowerup(const btVector3 &pos,
                         const float &angle) noexcept;

    void renderPlatforms() noexcept;
    void renderPowerups() noexcept;

    TimerManager m_timerManager;

template<typename... Components>
auto GetAllEntitiesWith()
{
    return registry.view<Components...>();
}
};
