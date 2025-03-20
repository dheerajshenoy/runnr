#include "runnr.hpp"
#include <raylib.h>

runnr::runnr()
{
    InitWindow(800, 600, "RunnR");
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // DisableCursor();
    SceneManager::GetInstance().currentScene = new GameScene();
}

void runnr::run() noexcept
{
    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        SceneManager::GetInstance().currentScene->update(dt);
        SceneManager::GetInstance().currentScene->render();
    }
}
