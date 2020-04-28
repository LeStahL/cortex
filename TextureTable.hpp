#pragma once

#include "Texture.hpp"

class TextureTable
{
    public:
    TextureTable();

    void addTexture(Texture *texture);
    bool hasTexture(const char *name);
    Texture *textureByName(const char *name);

    Texture **textures;
    int nTextures;
};