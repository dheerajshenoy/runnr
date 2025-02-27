#pragma once

#include <vector>
#include <raylib.h>
#include <iostream>
#include <memory>
#include "Player.hpp"
#include "Floor.hpp"
#include "SceneManager.hpp"
#include "MenuScene.hpp"
#include "GameScene.hpp"

class Runnr {

public:

    static Runnr& GetInstance() {
        static Runnr instance;  // Singleton instance
        return instance;
    }

    void run() noexcept;
    void initSettings() noexcept;
    void initScenes() noexcept;
    void update(const float &dt) noexcept;
    void exit() noexcept;

private:
    Runnr() noexcept;
    ~Runnr() noexcept;
    Runnr(const Runnr&) = delete;
    Runnr& operator=(const Runnr&) = delete;
    Runnr(Runnr&&) = delete;
    Runnr& operator=(Runnr&&) = delete;
    int m_wwidth = 800, m_wheight = 600;
    float m_gravity;
    float m_fps = 144.0f;
    int m_score = 0;
    bool m_fps_shown;
    bool m_paused = false;
    bool m_game_over = false;
    bool m_game_close = false;
};
