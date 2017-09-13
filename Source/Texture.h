#pragma once

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>

#include "stb/stb_image.h"

class Texture
{
public:

    // Loads the texture and fills all the member variables with proper values
    bool LoadTexture(const char* filename);

    const char* filename;
    int width;
    int height;
    GLuint textureID;
};
