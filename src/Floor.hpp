#pragma once

#include <raylib.h>

class Floor {

public:
    Floor() noexcept;
    ~Floor() noexcept;

void setSize(const Vector3 &size) noexcept;
inline Vector3 size() noexcept { return m_size; }
inline void setSpeed(const float &speed) noexcept { m_speed = speed; }
inline float speed() const noexcept { return m_speed; }
inline void setPos(const Vector3 &pos) noexcept { m_pos = pos; }
inline Vector3 pos() const noexcept { return m_pos; }
inline void setColor(const Color &color) noexcept { m_color = color; }
inline Color color() const noexcept { return m_color; }

void update(const float &dt) noexcept;
void render() noexcept;
inline bool is_off_screen() noexcept {
    return m_pos.z + m_size.z * 0.5 >= 75;
}

BoundingBox bbox() const noexcept;

private:
    Mesh m_mesh;
    Model m_model;
    Vector3 m_pos;
    Vector3 m_size;
    float m_speed;
    BoundingBox m_bbox;
    Color m_color;
};
