#pragma once

#include "Texture.hpp"
#include "TextureTable.hpp"
#include "Framebuffer.hpp"
#include "FramebufferTable.hpp"

class Pass
{
    public:
    Pass(TextureTable *textureTable);

    void addInput(const char *textureName);
    TextureTable *textureTable;
    Texture *inputs;
    int nInputs;
    // GLint outputFramebuffer,
        // *inputTextures;
};
