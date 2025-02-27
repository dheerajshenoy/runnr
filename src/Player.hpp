#pragma once

#include <raylib.h>
#include "Floor.hpp"
#include <vector>

class Player {

public:
    Player() noexcept;
    ~Player() noexcept;
    void update(const float &dt,
                const std::vector<Floor*>& floors) noexcept;
    void render() noexcept;
    void jump() noexcept;
    void setGravity(const float &g) noexcept { m_gravity = g; }
    float gravity() const noexcept { return m_gravity; }
    void setBBoxVisible(const bool &state) noexcept;
    BoundingBox bbox() const noexcept;
    inline void setIsOnGround(const bool &state) noexcept {
        m_is_on_ground = state;
    }
inline bool isOnGround() const noexcept { return m_is_on_ground; }

inline void setPos(const Vector3 &pos) noexcept { m_pos = pos; }
inline Vector3 pos() const noexcept { return m_pos; }

private:
    float m_speed;
    Vector3 m_pos;
    Mesh m_mesh;
    Model m_model;
    Vector3 m_size;
    Material m_material;
    float m_speedY;
    float m_gravity;
    float m_jump_force;
    bool m_is_on_ground;
    bool m_bbox_shown;
    BoundingBox m_bbox;
    Sound m_jump_sound;
};
