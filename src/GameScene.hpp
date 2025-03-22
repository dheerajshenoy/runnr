#pragma once

#include "Components.hpp"
#include "Scene.hpp"

#include "entt/entt.hpp"
#include "SceneManager.hpp"
#include "Player.hpp"
#include <raylib.h>
#include <raymath.h>
#include "bullet.hpp"
#include <rlgl.h>
#include <algorithm>
#include "Box.hpp"
#include <time.h>
#include <unordered_map>
#include "PlayerCollisionCallback.hpp"
#include "Entity.hpp"
#include "TimerManager.hpp"
#include <unordered_set>
#include "utils.hpp"
#include "ColorRegistry.hpp"

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

    Vector3 m_lightDir { (Vector3){ 0.35f, -1.0f, 0.35f } };
    Color m_lightColor { WHITE };
    float m_ambient[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    Shader m_shadowShader;

    int m_lightVPLoc;
    int m_shadowMapLoc;
    int m_shadowMapResolution = 2048; // Higher size = more accurate shadows
    RenderTexture2D m_shadowMapTexture;
    void shaderRun() noexcept;
    Matrix m_lightView;
    Matrix m_lightProj;
    RenderTexture2D LoadShadowmapRenderTexture(const int &width,
                                               const int &height) noexcept;
    void UnloadShadowmapRenderTexture(RenderTexture2D target) noexcept;
    void cameraFollowPlayer() noexcept;

    void updateTrail() noexcept;
    void renderTrail() noexcept;

    Vector3 lastPlatformPos;
    std::vector<Vector3> m_trailPositions;
    const int m_maxTrailLength = 50;

    float lastSpawnZ { 50.0f };
    entt::entity lastPlatformHandle { entt::null };
    float m_length { 50.0f };
    float m_angle { 0.0f };

    float m_platform_speed { 10.0f };

    time_t m_seed;

    Box *box { nullptr };

    Entity CreatePlatform(const btVector3 &pos,
                          const btVector3 &size,
                          const float &speedZ,
                          const float &angle) noexcept;

    Entity CreatePowerup(const btVector3 &pos,
                         const float &angle) noexcept;

    void renderPlatforms() noexcept;
    void renderPowerups() noexcept;
    void renderActivePowerups() noexcept;

    TimerManager m_timerManager;

    std::unordered_set<PowerupComponent::PowerupType> m_active_powerups {};

    void removePowerup(const PowerupComponent::PowerupType &type) noexcept;
    void addPowerup(const PowerupComponent::PowerupType &type) noexcept;

    void ShapeShiftPlayer(const Player::ShapeType &type) noexcept;

    template<typename... Components>
    auto GetAllEntitiesWith() noexcept {
        return registry.view<Components...>();
    }

    ColorRegistry m_colorRegistry;

    void shapeShiftToBox() noexcept;
    void shapeShiftToSphere() noexcept;

};
