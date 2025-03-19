#pragma once

#include "Scene.hpp"

class SceneManager {

public:
    Scene* currentScene;
    int width;
    int height;

static SceneManager& GetInstance() {
    static SceneManager instance;
    return instance;
}

void setScene(Scene *scene) {
    delete currentScene;
    currentScene = scene;
}

private:
    SceneManager() = default;
    ~SceneManager() {
        delete currentScene;
    }
};
