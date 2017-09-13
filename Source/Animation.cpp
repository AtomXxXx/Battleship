#include "Animation.h"
#include "GraphicsHelpers.h"

bool Animation::Init(const char* filename, int totalFrames, int frameRate, int numFramesX, int numFramesY)
{
    spriteSheet = new Texture();
    if(!spriteSheet->LoadTexture(filename))
    {
        delete spriteSheet;
        spriteSheet = NULL;
        return false;
    }

    Init(spriteSheet, totalFrames, frameRate, numFramesX, numFramesY);

    return true;
}

void Animation::Init(Texture* spriteSheet, int totalFrames, int frameRate, int numFramesX, int numFramesY)
{
    this->spriteSheet = spriteSheet;

    this->playing = false;
    this->loop = false;
    this->currentFrameX = 0;
    this->currentFrameY = 0;
    this->posX = 0.0f;
    this->posY = 0.0f;
    this->angle = 0.0f;
    this->frameRate = frameRate;
    this->numFramesX = numFramesX;
    this->numFramesY = numFramesY;
    this->totalFrames = totalFrames;
    this->frameWidth = 1.0 / numFramesX;
    this->frameHeight = 1.0 / numFramesY;
    this->elapsedTime = 0.0;

    verts[0][0] = -0.5f;
    verts[0][1] = 0.5f;

    verts[1][0] = 0.5f;
    verts[1][1] = 0.5f;

    verts[2][0] = 0.5f;
    verts[2][1] = -0.5f;

    verts[3][0] = -0.5f;
    verts[3][1] = -0.5f;
}

void Animation::SetVertices(float width, float height)
{
    verts[0][0] = -width / 2.0;
    verts[0][1] = height / 2.0;

    verts[1][0] = width / 2.0;;
    verts[1][1] = height / 2.0;

    verts[2][0] = width / 2.0;;
    verts[2][1] = -height / 2.0;

    verts[3][0] = -width / 2.0;;
    verts[3][1] = -height / 2.0;
}

void Animation::SetPosition(float posX, float posY)
{
    this->posX = posX;
    this->posY = posY;
}
void Animation::SetAngle(float angle)
{
    this->angle = angle;
}

void Animation::Play(bool loop)
{
    this->loop = loop;
    this->playing = true;
    currentFrameX = 0;
    currentFrameY = 0;
}

bool Animation::GetUV(int frameX, int frameY, float uv[4][2])
{
    if(frameX < 0 || frameY < 0 || frameX >= numFramesX || frameY >= numFramesY)
        return false;

    uv[0][0] = 0.0f + frameWidth * frameX;
    uv[0][1] = 0.0f + frameHeight * frameY;

    uv[1][0] = frameWidth + frameWidth * frameX;
    uv[1][1] = 0.0f + frameHeight * frameY;

    uv[2][0] = frameWidth + frameWidth * frameX;
    uv[2][1] = frameHeight + frameHeight * frameY;

    uv[3][0] = 0.0f + frameWidth * frameX;
    uv[3][1] = frameHeight + frameHeight * frameY;

    return true;
}

void Animation::UpdateFrame(double dt)
{

    if(!playing) return;

    double deltaTimeFrames = 1.0 / frameRate;

    if(elapsedTime < deltaTimeFrames)
    {
        elapsedTime += dt;
        return;
    }

    int framesToSkip = elapsedTime / deltaTimeFrames + 0.5f;
    elapsedTime = 0.0;

    int currentFrameNum = currentFrameX + currentFrameY * numFramesX;
    currentFrameNum += framesToSkip;
    currentFrameY = currentFrameNum / numFramesX;
    currentFrameX = currentFrameNum - currentFrameY * numFramesX;
    if(currentFrameNum >= totalFrames)
    {
        if(loop)
        {
            currentFrameNum = currentFrameNum % totalFrames;
            currentFrameY = currentFrameNum / numFramesX;
            currentFrameX = currentFrameNum - currentFrameY * numFramesX;
        }
        else
        {
            currentFrameX = 0;
            currentFrameY = 0;
            playing = false;
        }
    }
}

void Animation::DrawFrame()
{
    if(!playing) return;

    float uv[4][2];
    GetUV(currentFrameX, currentFrameY, uv);

    GraphicsHelpers::DrawTexturedRectangle(verts, uv, spriteSheet->textureID, posX, posY, angle);
}
