#include "GameScene.hpp"
#include <raylib.h>

GameScene::GameScene()
:   m_game_over(false),
    m_info_overlay(false),
    m_score(0)
{
    m_player = new Player();
    m_player->setPos({0, 10, 0});
    Floor *static_floor = new Floor();
    static_floor->setSpeed(4);
    static_floor->setPos({0, 0, 0});
    static_floor->setSize({50, 1, 10});
    m_floor_list.push_back(static_floor);
    initCamera();

    m_jump_sound = LoadSound("../resources/sounds/Coffee1.mp3");

}

GameScene::~GameScene() {
    delete m_player;
    UnloadSound(m_jump_sound);
    // UnloadSound(m_on_land_sound);
}

void GameScene::initCamera() noexcept {
    m_camera.position   = (Vector3){ 0.0f, 4.0f, 5.0f }; // Camera position
    m_camera.target     = (Vector3){ 0.0f, 0.0f, 0.0f }; // Camera looking at point
    m_camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
    m_camera.fovy       = 90.0f; // Camera field-of-view Y
    m_camera.projection = CAMERA_PERSPECTIVE; // Camera projection type
}

void GameScene::update(const float &dt) {
    handleInput();
    m_floor_list.erase(std::remove_if(m_floor_list.begin(), m_floor_list.end(),
                                      [](Floor* b) { return b->is_off_screen(); }),
                       m_floor_list.end());

    for (const auto &floor : m_floor_list)
        floor->update(dt);

    m_player->update(dt, m_floor_list);

    if (m_player->pos().y <= -10) {
        SceneManager::GetInstance().changeScene(new GameOverScene());
    }

    m_score += 1 * dt;
}

void GameScene::render() {
    UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    BeginMode3D(m_camera);
    for(const auto &floor : m_floor_list)
        floor->render();
    m_player->render();
    EndMode3D();
    if (m_info_overlay)
        drawInfo();
    DrawText(TextFormat("Score: %d", m_score), 10, 10, 20, RAYWHITE);
}

void GameScene::handleInput() noexcept {
    if (IsKeyPressed(KEY_F3)) {
        m_info_overlay = !m_info_overlay;
    }
}

void GameScene::drawInfo() noexcept {
    auto player_pos = m_player->pos();
    DrawText(TextFormat("Position = x: %.2f, y: %.2f, z: %.2f", player_pos.x, player_pos.y, player_pos.z), 10, 10, 20, RAYWHITE);
}
