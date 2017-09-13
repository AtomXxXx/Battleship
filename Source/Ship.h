#pragma once

#include "Grid.h"

class Ship
{
public:

    void Draw(Grid& grid, bool selected);
    int length; // number of slots it'll take up on grid
    bool isVertical;
    int numPartsDestoryed;

    int gridPosX;
    int gridPosY;

    float defaultPosX; // position of ship when it isn't on the grid
    float defaultPosY;

    float posX;
    float posY;
    float width; // actual width and height of the ship in meters
    float height;

    GLuint textureID;
};
