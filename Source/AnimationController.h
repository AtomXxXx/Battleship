#pragma once

#include <vector>
#include <list>
#include <string.h>

#include "Animation.h"

// Controls all the animations currently playing
class AnimationController
{
public:

    AnimationController()
    {
        blocking = false;
    }
    // Creates a new animation and starts playing it
    void InstantiateAnimation(AnimationInfo animInfo, float posX, float posY, float width, float height, bool loop, bool blocking);
    // Finds the sprite sheet from the vector of textures
    bool FindSpriteSheet(const char* filename, Texture** sheet);
    // Updates all the animations in the list
    void Update(double dt);
    // Renders all the animation in the list
    void Draw();
    bool IsBlockingAnimationPlaying();

    std::vector<Texture*> spriteSheets;
    std::list<Animation*> animations;
    bool blocking; // true if a blocking animation is playing (used to 'pause' gameplay)
};
