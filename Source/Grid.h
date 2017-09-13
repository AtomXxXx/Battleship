#pragma once

#include "GraphicsHelpers.h"
#include <math.h>

class Grid
{
public:

    void Init();
    // Updates the sin wave lines
    void Update(double dt);
    // Draws the grid
    void DrawGrid();
    // Gets 'height' of the sine wave and the middle of a grid position
    void GetOffsetXY(int gridX, int gridY, float* offX, float* offY);
    // Converts the integer grid position to its position in the world
    void GetPositionWorld(int gridX, int gridY, float* x, float* y);

    float posX; //top left
    float posY;
    float width; // board width in meters
    float height;
    int numBoxesX;
    int numBoxesY;
    float lineThickness;
    float color[3];
    float boxWidth;
    float boxHeight;

    // Wavy lines related variables
    float amplitude;
    float numWaveUnitsPerLineStrip;
    int numVerts;
    float startAngle;
    float angleSpeed;

};
