#pragma once

#include "entt/entt.hpp"

class Scene {
protected:
    entt::registry registry;

public:
    Scene() {}
    virtual ~Scene() = default;
    virtual void render() = 0;
    virtual void update(const float &dt) = 0;

friend class Entity;
};
