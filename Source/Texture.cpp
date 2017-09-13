#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool Texture::LoadTexture(const char* filename)
{
    int channels;
    unsigned char* texels = stbi_load(filename, &width, &height, &channels, 4);

    if(texels == NULL)
    {
        printf("stb_image could not load image\n");
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glBindTexture(GL_TEXTURE_2D, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(glGetError() != GL_NO_ERROR)
    {
        printf("Could not load texture\n");
        return false;
    }
    STBI_FREE(texels);

    //this->filename = new char[strlen(filename)];
    //strcpy(this->filename, filename);
    this->filename = filename;

    return true;
}
