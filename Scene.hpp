#pragma once

#include "Pass.hpp"
#include "Demo.hpp"

class Scene
{
    public:
    Scene(Demo *demo, const char *name, float startTime, float endTime);

    void addPass(Pass *pass);
    bool isShownAt(float time) const;
    void setup();
    void draw();
    float sceneTime(float globalTime) const;

    float startTime,
        endTime;
    
    Pass **passes;
    int nPasses;
    char *name;
    Demo *demo;
};