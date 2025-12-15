#pragma once

#include <GL/freeglut.h>
#include <FreeImage.h>
#include <string>

class TextureHandler {
public:
    static GLuint createTexture(const std::string& filename);
    static void drawTexture(GLuint textureID, float left, float right, float bottom, float top);
};
