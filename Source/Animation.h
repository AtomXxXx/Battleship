#pragma once

#include "Texture.h"

// Contains the basic info of an animation
struct AnimationInfo
{
    void Init(const char* filename, int totalFrames, int frameRate, int numFramesX, int numFramesY)
    {
        this->filename = filename;
        this->frameRate = frameRate;
        this->totalFrames = totalFrames;
        this->numFramesX = numFramesX;
        this->numFramesY = numFramesY;
    }
    const char* filename;
    int totalFrames;
    int frameRate;
    int numFramesX;
    int numFramesY;
};

// One object is created per animation
class Animation
{
public:

    // Loads the sprite sheet and initializes
    bool Init(const char* filename, int totalFrames, int frameRate, int numFramesX, int numFramesY);
    // Initializes using the already loaded sprite sheet
    void Init(Texture* spriteSheet, int totalFrames, int frameRate, int numFramesX, int numFramesY);
    // Calculates and sets the next frame of animation
    void UpdateFrame(double dt);
    // Starts the animation
    void Play(bool loop);
    // Calculates the uv of a particular frame
    bool GetUV(int frameX, int frameY, float uv[4][2]);
    // Draws the current frame
    void DrawFrame();
    void SetVertices(float width, float height);
    void SetPosition(float posX, float posY);
    void SetAngle(float angle);

    Texture* spriteSheet;
    double elapsedTime;
    int frameRate;
    int totalFrames;
    int numFramesX;
    int numFramesY;
    int currentFrameX;
    int currentFrameY;
    float frameWidth;
    float frameHeight;
    float posX;
    float posY;
    float angle;
    float verts[4][2];
    bool playing;
    bool loop;
    bool blocking;
};
