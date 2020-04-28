#pragma once

#include "Framebuffer.hpp"

class FramebufferTable
{
    public:
    FramebufferTable();

    void addFramebuffer(Framebuffer *framebuffer);
    Framebuffer *framebufferByName(const char *name) const;
    bool hasFramebuffer(const char *name) const;

    int nFramebuffers;
    Framebuffer *framebuffers;
};