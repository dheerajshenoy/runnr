#pragma once

#include <raylib.h>
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "Button.hpp"
#include "Runnr.hpp"

class GameOverScene : public Scene {

public:
    GameOverScene();
    ~GameOverScene();
    void render() override;
    void update(const float &dt) override;

private:
    Button *m_restart_btn;
    Button *m_exit_btn;
    Sound m_game_over_sound;
};
