#pragma once

#include <Windows.h>
#include "gl/GL.h"

#include "Build.gen.hpp"
#include "Shader.hpp"

class Program
{
public:
    Program();
    
    void attachShader(Shader *shader);
    void link();
    void use();
    void handleUniform(const char *uniformName);
    int uniformIndex(const char *uniformName) const;
    void uniform1f(const char *uniformName, float value);
    void uniform1i(const char *uniformName, int value);
    void uniform2f(const char *uniformName, float value1, float value2);

    char **uniformNames;
    int *uniformLocations;
    int nUniforms;

    GLuint handle;
    Shader **shaders;
    int nShaders;
#ifdef DEBUG
    bool hasLinkError();
    char *linkError();
#endif // DEBUG
};
