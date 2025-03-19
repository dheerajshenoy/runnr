#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <raymath.h>

class Player {
    public:

    Player(const btVector3 &position, btDiscreteDynamicsWorld*);
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

    void render() noexcept;
    Model model;
    void move(const MoveDirection &dir) noexcept;
    void jump() noexcept;
    bool isOnGround() noexcept;

    btTransform position() noexcept;

    btVector3 jumpForce;

    private:
    float m_restitution { 0.8f };
};
