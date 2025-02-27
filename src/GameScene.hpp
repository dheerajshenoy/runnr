#pragma once

#include "Scene.hpp"
#include "Floor.hpp"
#include "Player.hpp"
#include <vector>
#include <algorithm>
#include "SceneManager.hpp"
#include "GameOverScene.hpp"

class GameScene : public Scene {

public:
    GameScene();
    ~GameScene();
    void initCamera() noexcept;
    void render() override;
    void update(const float &dt) override;


private:
    void handleInput() noexcept;
    void drawInfo() noexcept;
    Camera m_camera;
    Player *m_player;
    std::vector<Floor*> m_floor_list;
    bool m_game_over;
    bool m_info_overlay;
    int m_score;
    Sound m_jump_sound;
    Sound m_on_land_sound;
};
