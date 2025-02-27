#include "Floor.hpp"
#include <raylib.h>

Floor::Floor() noexcept
: m_pos({0, 0, 0}),
    m_size({0, 0, 0}),
    m_speed(40.0f)
{
    m_mesh = GenMeshCube(m_size.z, m_size.y, m_size.x);
    m_model = LoadModelFromMesh(m_mesh);
    m_bbox = GetMeshBoundingBox(m_mesh);
}

void Floor::render() noexcept {
    DrawModel(m_model, m_pos, 1.0f, PINK);
}

void Floor::update(const float &dt) noexcept {
    m_pos.z += m_speed * dt;
}

BoundingBox Floor::bbox() const noexcept {
    return (BoundingBox) {
        (Vector3){ m_bbox.min.x + m_pos.x, m_bbox.min.y + m_pos.y, m_bbox.min.z + m_pos.z },
        (Vector3){ m_bbox.max.x + m_pos.x, m_bbox.max.y + m_pos.y, m_bbox.max.z + m_pos.z }
    };
}

Floor::~Floor() noexcept {
    UnloadModel(m_model);
    UnloadMesh(m_mesh);
}

void Floor::setSize(const Vector3 &size) noexcept {
    m_size = size;
    m_mesh = GenMeshCube(m_size.z, m_size.y, m_size.x);
    m_model = LoadModelFromMesh(m_mesh);
    m_bbox = GetMeshBoundingBox(m_mesh);
}
