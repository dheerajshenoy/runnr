#pragma once

class Scene {

public:
    virtual void update(const float &dt) = 0;
    virtual void render() = 0;
    virtual ~Scene()
    {}
};
