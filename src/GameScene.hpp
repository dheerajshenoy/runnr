#pragma once

#include "Scene.hpp"
#include "Floor.hpp"
#include "Player.hpp"
#include <vector>
#include <algorithm>
#include "SceneManager.hpp"
#include "GameOverScene.hpp"
#include "Timer.hpp"
#include "Button.hpp"
#include "Runnr.hpp"
#include <raymath.h>
#include <rlgl.h>

class GameScene : public Scene {

public:
    GameScene();
    ~GameScene();
    void initCamera() noexcept;
    void render() override;
    void update(const float &dt) override;
    inline bool isPaused() const noexcept { return m_paused; }

private:
    RenderTexture2D LoadShadowmapRenderTexture(const int &width, const int &height) noexcept;
    void UnloadShadowmapRenderTexture(const RenderTexture2D &target) noexcept;
    void handleInput() noexcept;
    void renderInfo() noexcept;
    void renderPausedMenu() noexcept;
    void initFloorSpawning() noexcept;
    void initShader() noexcept;
    Camera m_camera;
    Player *m_player;
    std::vector<Floor*> m_floor_list;
    std::vector<Rectangle> m_rects;
    bool m_game_over;
    bool m_info_overlay;
    int m_score;
    Sound m_jump_sound;
    Sound m_on_land_sound;
    Timer m_timer;
    bool m_paused = false;
    Button* m_resume_btn,
        *m_exit_btn;

// Shadow Shader Stuff
Shader m_shadow_shader;
Vector3 m_lightDir;
RenderTexture2D m_shadowMap;
int m_lightVPLoc;
int m_shadowMapLoc;
Camera3D m_lightCam = (Camera3D){ 0 };
};
