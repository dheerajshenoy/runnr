#pragma once

#include "bullet.hpp"
#include <string>
#include <raylib.h>

class Platform;

struct TagComponent
{
    TagComponent(const std::string &tag = std::string()) : Tag(tag)
        {}

    TagComponent(const TagComponent &other) = default;

    std::string Tag;
};

struct RenderComponent
{
    union
    {
        btVector3 size;
        btScalar radius;
    } dimension;

    Model model;

    Color color;
};

struct PlatformComponent
{
    enum class PlatformType
    {
        Regular = 0
    };

    PlatformType type { PlatformType::Regular };

    PlatformComponent() = default;
    PlatformComponent(const PlatformComponent &other) = default;
};

struct PowerupComponent
{
    enum class PowerupType
    {
        Jump = 0,
        Fast,
        Slow,
    };

    PowerupType Type { PowerupType::Jump };
};

struct EnemyComponent
{};

struct RigidBodyComponent
{
    btRigidBody* body { nullptr };
    btCollisionShape* shape { nullptr };
    btMotionState* motionState { nullptr };

    enum class ShapeType
    {
        Box = 0,
        Sphere
    };

    ShapeType shapeType { ShapeType::Box };

    RigidBodyComponent() = default;
    RigidBodyComponent(const RigidBodyComponent &other) = default;
};
