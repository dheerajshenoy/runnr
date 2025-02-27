#include "Runnr.hpp"
#include "src/SceneManager.hpp"

Runnr::Runnr() noexcept
:   m_fps_shown(false)
{
    InitWindow(m_wwidth, m_wheight, "RunnR");
    InitAudioDevice();
    initSettings();
    initScenes();
}

Runnr::~Runnr() noexcept {
    ShowCursor();
    CloseAudioDevice();
    CloseWindow();
}

void Runnr::exit() noexcept {
    m_game_close = true;
}

void Runnr::initSettings() noexcept {
    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    SetTargetFPS(m_fps);
}


void Runnr::run() noexcept {

    while (!m_game_close) {

        float dt = GetFrameTime();

        if (IsWindowResized()) {
            m_wwidth = GetScreenWidth();
            m_wheight = GetScreenHeight();
        }

        this->update(dt);
        SceneManager::GetInstance().update(dt);

        BeginDrawing();

            ClearBackground(BLACK);

        SceneManager::GetInstance().render();

        EndDrawing();
    }
}

void Runnr::update(const float &dt) noexcept {
    if (IsKeyPressed(KEY_F)) {
        m_fps_shown = !m_fps_shown;
    }

    // DrawText(TextFormat("x=%.2f, y=%.2f, z=%.2f", pos.x, pos.y, pos.z), 10, 10, 20, RAYWHITE);
    if (m_fps_shown) {
        DrawText(TextFormat("FPS: %d", GetFPS()), m_wwidth - 100, 20, 20, RAYWHITE);
    }
}

void Runnr::initScenes() noexcept {
    SceneManager::GetInstance().changeScene(new MenuScene());
}
