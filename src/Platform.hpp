#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <raymath.h>

class Platform
{

    public:
    Platform(const btVector3 &position,
             const btVector3 &dimension,
             const float &speed = 10.0f);
    btRigidBody *body { nullptr };
    btTransform transform;
    Model model;
    void render() noexcept;
    bool isOffScreen(const float &zOffset) noexcept;
    void update(const float &dt) noexcept;
    void cleanup(btDiscreteDynamicsWorld*) noexcept;
    void setRotation(const btVector3 &axis, const btScalar &angle) noexcept;

    private:
    float m_speed { 10.0f };
    float m_restitution { 0.3f };
    btCollisionShape* colShape { nullptr };


};
