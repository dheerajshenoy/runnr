#pragma once

#include "Scene.hpp"

class SceneManager {
public:
    static SceneManager& GetInstance() {
        static SceneManager instance;  // Singleton instance
        return instance;
    }

void changeScene(Scene* newScene) {
    if (currentScene) {
        delete currentScene;
    }
    currentScene = newScene;
}

void update(const float &dt) {
    if (currentScene)
        currentScene->update(dt);
}

void render() {
    if (currentScene)
        currentScene->render();
}

private:
    Scene* currentScene = nullptr;

SceneManager() = default;  // Private constructor
~SceneManager() { delete currentScene; }

// Delete copy/move constructors to enforce singleton pattern
SceneManager(const SceneManager&) = delete;
SceneManager& operator=(const SceneManager&) = delete;
SceneManager(SceneManager&&) = delete;
SceneManager& operator=(SceneManager&&) = delete;
};
