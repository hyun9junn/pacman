#include "TextureHandler.h"
#include <iostream>

GLuint TextureHandler::createTexture(const std::string& filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
    if (format == -1) {
        std::cerr << "Could not find image: " << filename << std::endl;
        exit(-1);
    }
    else if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(filename.c_str());
        if (!FreeImage_FIFSupportsReading(format)) {
            std::cerr << "Detected image format cannot be read!" << std::endl;
            exit(-1);
        }
    }

    FIBITMAP* bitmap = FreeImage_Load(format, filename.c_str());
    int bitsPerPixel = FreeImage_GetBPP(bitmap);
    // std::cout << "Source image has " << bitsPerPixel << " bits per pixel." << std::endl;

    FIBITMAP* bitmap32 = bitmap;
    if (bitsPerPixel != 32) {
        // std::cout << "Converting to 32-bits." << std::endl;
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int imageWidth = FreeImage_GetWidth(bitmap32);
    int imageHeight = FreeImage_GetHeight(bitmap32);

    GLubyte* textureData = FreeImage_GetBits(bitmap32);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL texture creation error: " << error << std::endl;
        exit(-1);
    }

    FreeImage_Unload(bitmap32);
    if (bitmap != bitmap32) {
        FreeImage_Unload(bitmap);
    }
    return textureID;
}

void TextureHandler::drawTexture(GLuint textureID, float left, float right, float bottom, float top) {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(left, bottom);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(right, bottom);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(right, top);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(left, top);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
