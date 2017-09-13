#include "AnimationController.h"

bool AnimationController::FindSpriteSheet(const char* filename, Texture** sheet)
{
    for(std::vector<Texture>::size_type i = 0; i != spriteSheets.size(); i++)
    {
        if(strcmp(spriteSheets[i]->filename, filename) == 0)
        {
            *sheet = spriteSheets[i];
            return true;
        }
    }
    return false;
}

void AnimationController::InstantiateAnimation(AnimationInfo animInfo, float posX, float posY, float width, float height, bool loop, bool blocking)
{
    Texture* sheet = NULL;
    Animation* anim = new Animation;

    if(!FindSpriteSheet(animInfo.filename, &sheet))
    {
        sheet = new Texture();
        sheet->LoadTexture(animInfo.filename);
        spriteSheets.push_back(sheet);
    }

    anim->Init(sheet, animInfo.totalFrames, animInfo.frameRate, animInfo.numFramesX, animInfo.numFramesY);
    anim->blocking = blocking;
    anim->SetPosition(posX, posY);
    anim->SetVertices(width, height);
    anim->Play(loop);
    animations.push_back(anim);

    if(blocking)
        this->blocking = true;
}

void AnimationController::Update(double dt)
{
    // Update all animations and remove from list if its not playing
    for(std::list<Animation*>::iterator it = animations.begin(); it != animations.end(); ++it)
    {
        Animation* anim = *it;
        anim->UpdateFrame(dt);

        if(anim->playing == false)
        {
            it = animations.erase(it);
            if(animations.empty())
                break;
            it--;
        }
    }

    // Check whether a blocking animation is playing
    blocking = false;
    for(std::list<Animation*>::iterator it = animations.begin(); it != animations.end(); ++it)
    {
        Animation* anim = *it;
        if(anim->blocking)
            blocking = true;
    }
}

void AnimationController::Draw()
{
    for(std::list<Animation*>::iterator it = animations.begin(); it != animations.end(); ++it)
    {
        Animation* anim = *it;
        anim->DrawFrame();
    }
}

bool AnimationController::IsBlockingAnimationPlaying()
{
    return blocking;
}
