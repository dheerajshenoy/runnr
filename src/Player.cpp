#include "Player.hpp"
#include <raylib.h>
#include <iostream>

Player::Player() noexcept
:   m_speed(14.0f),
    m_size({1, 1, 1}),
    m_speedY(0.0f),
    m_gravity(10.0f),
    m_jump_force(-7.5f),
    m_is_on_ground(false),
    m_pos({0, 0, 0}),
    m_bbox_shown(false)
{
    m_mesh = GenMeshCube(m_size.x, m_size.y, m_size.z);
    m_model = LoadModelFromMesh(m_mesh);

    m_pos = {1, m_size.y, 1};

    m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    m_bbox = GetMeshBoundingBox(m_mesh);

    m_jump_sound = LoadSound("../resources/sounds/Modern3.mp3");
}

void Player::update(const float &dt, const std::vector<Floor*>& floors) noexcept
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        m_pos.x -= m_speed * dt;
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        m_pos.x += m_speed * dt;
    }

    if (IsKeyPressed(KEY_SPACE) && m_is_on_ground) {
        jump();
    }

    if (IsKeyPressed(KEY_B)) {
        setBBoxVisible(!m_bbox_shown);
    }

    if (!m_is_on_ground) {
        m_speedY += m_gravity * dt;
        m_pos.y -= m_speedY * dt;

        // if (m_pos.y <= m_jump_force) {
        //     m_pos.y = m_jump_force;
        //     m_speedY = 0;
        // }
    }

    for (const auto &floor : floors) {
        if (CheckCollisionBoxes(bbox(), floor->bbox())) {
            m_is_on_ground = true;
            // TODO: Implement multi face collision detection!
            m_pos.y = floor->pos().y + m_size.y;
            break;
        } else {
            m_is_on_ground = false;
        }
    }
}

void Player::jump() noexcept
{
    m_speedY = m_jump_force;
    m_is_on_ground = false;
    PlaySound(m_jump_sound);
}

void Player::render() noexcept
{
    DrawModel(m_model, m_pos, 1.0f, WHITE);

    DrawCubeWires(m_pos, m_size.z, m_size.y, m_size.x, BLACK);

    if (m_bbox_shown) {
        auto bbox = (BoundingBox) {
            (Vector3){ m_bbox.min.x + m_pos.x, m_bbox.min.y + m_pos.y, m_bbox.min.z + m_pos.z },
            (Vector3){ m_bbox.max.x + m_pos.x, m_bbox.max.y + m_pos.y, m_bbox.max.z + m_pos.z }
        };
        DrawBoundingBox(bbox, GOLD);
    }
}

BoundingBox Player::bbox() const noexcept
{
    return (BoundingBox) {
        (Vector3){ m_bbox.min.x + m_pos.x, m_bbox.min.y + m_pos.y, m_bbox.min.z + m_pos.z },
        (Vector3){ m_bbox.max.x + m_pos.x, m_bbox.max.y + m_pos.y, m_bbox.max.z + m_pos.z }
    };
}

Player::~Player() noexcept
{
    UnloadModel(m_model);
}


void Player::setBBoxVisible(const bool &state) noexcept {
    m_bbox_shown = state;
}
