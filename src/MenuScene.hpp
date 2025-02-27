#pragma once

#include "Scene.hpp"
#include <raylib.h>
#include "Button.hpp"
#include "SceneManager.hpp"
#include "GameScene.hpp"

class MenuScene : public Scene {

public:
    MenuScene();
    ~MenuScene();
    void render() override;
    void update(const float &dt) override;

private:
    Button *m_play_btn,
        *m_settings_btn,
        *m_exit_btn;
};
