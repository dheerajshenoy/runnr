#include "GameOverScene.hpp"
#include "src/GameScene.hpp"

GameOverScene::GameOverScene()
{
    m_game_over_sound = LoadSound("../resources/sounds/Error.mp3");
    m_restart_btn = new Button("Restart", 10, 40, 100, 40);
    m_exit_btn = new Button("Exit", 10, 90, 100, 40);
    m_restart_btn->pressed([]() {
        SceneManager::GetInstance().changeScene(new GameScene());
    });
    m_exit_btn->pressed([]() {
        Runnr::GetInstance().exit();
    });
}

GameOverScene::~GameOverScene()
{
    delete m_restart_btn;
    delete m_exit_btn;
    UnloadSound(m_game_over_sound);
}

void GameOverScene::render()
{
    m_restart_btn->render();
    m_exit_btn->render();
    DrawText("Game Over", 10, 10, 20, RAYWHITE);
}

void GameOverScene::update(const float &dt)
{
    m_restart_btn->update();
    m_exit_btn->update();
    PlaySound(m_game_over_sound);
}
