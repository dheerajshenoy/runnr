#pragma once

#include "bullet.hpp"
#include <raylib.h>
#include <raymath.h>

class Powerup
{
    public:
    enum class Type
    {
        JumpBoost = 0,
        Slow,
        Fast,
        ShapeShifter,
    };

    Powerup(const Type &type,
            const btVector3 &pos);
    ~Powerup();
    void render() noexcept;
    void update(const float &dt) noexcept;
    bool isOffScreen(const float &zOffset) noexcept;

    Model model;

    btTransform transform;
    btRigidBody *body { nullptr };

    private:
    Type m_type { Type::JumpBoost };
    btCollisionShape *m_shape { nullptr };
    float m_speed { 10.0f };
};
