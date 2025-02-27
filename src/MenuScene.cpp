#include "MenuScene.hpp"
#include <raylib.h>

MenuScene::MenuScene()
{
    m_play_btn = new Button("Play", 10.0, 100.0, 100.0, 50.0);
    m_settings_btn = new Button("Settings", 10.0, 170.0, 100.0, 50.0);
    m_exit_btn = new Button("Exit", 10.0, 240.0, 100.0, 50.0);

    m_play_btn->pressed([]() {
        SceneManager::GetInstance().changeScene(new GameScene());
    });

    m_settings_btn->pressed([&]() {
        m_settings_btn->setText("DD");
    });

    m_exit_btn->pressed([]() {
        Runnr::GetInstance().exit();
    });
}

void MenuScene::render()
{
    BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText("RunnR", 10, 20, 50, PURPLE);
    m_play_btn->render();
    m_settings_btn->render();
    m_exit_btn->render();

    EndDrawing();
}

void MenuScene::update(const float &dt)
{
    m_play_btn->update();
    m_settings_btn->update();
    m_exit_btn->update();
}

MenuScene::~MenuScene()
{
    delete m_play_btn;
    delete m_settings_btn;
    delete m_exit_btn;
}
