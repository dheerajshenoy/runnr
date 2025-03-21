#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Components.hpp"

class Scene;

class Player {
    public:

    Player(const btVector3 &position, btDiscreteDynamicsWorld*, Scene *scene);
    btCollisionShape* colShape { nullptr };
    btTransform transform;
    float mass { 1.0f };
    btRigidBody *body { nullptr };
    bool is_on_ground { false };
    btDiscreteDynamicsWorld *world { nullptr };

    enum class MoveDirection {
        LEFT = 0,
        RIGHT,
    };

    void SetJumpForce(const btVector3 &force) noexcept;
    void ResetJumpForce() noexcept;

    void update(const float &dt) noexcept;
    void render() noexcept;
    Model model;
    void move(const MoveDirection &dir) noexcept;
    void jump() noexcept;
    bool isOnGround() noexcept;
    bool isPlayerOnGround() noexcept;

    btTransform position() noexcept;

    btVector3 jumpForce;
    Scene *scene { nullptr };

    private:
    btVector3 m_defaultJumpForce;
    float m_restitution { 0.8f };

};
