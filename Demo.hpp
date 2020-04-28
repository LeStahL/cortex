#pragma once

#include "Scene.hpp"
#include "Window.hpp"
#include "LoadingBar.hpp"

class Demo
{
    Demo(LoadingBar *loadingBar, Window *window);

    // Setup functions
    void addScene(Scene *scene);
    int nScenes;
    Scene **scenes;

    // Control functions
    void play();
    void pause();
    void reset();
    void moveToScene(const char *sceneName);
    void moveToTime(float seconds);
    float globalTime();
    float sceneTime();
};