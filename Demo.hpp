#pragma once

class Demo
{
    Demo();

    // Setup functions
    // void addScene(Scene *scene);

    // Control functions
    void play();
    void pause();
    void reset();
    void moveToScene(const char *sceneName);
    void moveToTime(float seconds);
    float globalTime();
    float sceneTime();
};