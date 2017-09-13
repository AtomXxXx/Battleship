#include "Grid.h"

void Grid::DrawGrid()
{
    GraphicsHelpers::DrawLine(posX, posY, height, lineThickness, -90, color);
    GraphicsHelpers::DrawLine(posX + numBoxesX * boxWidth, posY, height, lineThickness, -90, color);
    GraphicsHelpers::DrawLine(posX, posY, width, lineThickness, 0, color);
    GraphicsHelpers::DrawLine(posX, posY - numBoxesY * boxHeight, width, lineThickness, 0, color);


    for(int lineX = 1; lineX < numBoxesX; lineX++)
    {
        //GraphicsHelpers::DrawLine(posX + x * boxWidth, posY, height, lineThickness, -90, color);
        float waveLength = height / numWaveUnitsPerLineStrip;
        float angle = startAngle;
        float *verts = new float[numVerts * 2];
        float gapBwVerts = height / (numVerts - 1);

        for(int vert = 0; vert < numVerts; vert++)
        {
            angle += 360 / (waveLength / gapBwVerts);
            if(angle >= 360) angle -= 360;
            float radian = angle * 3.1415 / 180;
            float offsetY = sin(radian) * amplitude;

            verts[0 + vert * 2] = gapBwVerts * vert;
            verts[1 + vert * 2] = 0.0 + offsetY;
        }

        GraphicsHelpers::DrawLineStrip(posX + lineX * boxWidth, posY, -90, verts, numVerts, lineThickness, color);

        delete []verts;
        verts = 0;
    }
    for(int lineY = 1; lineY < numBoxesY; lineY++)
    {
        //GraphicsHelpers::DrawLine(posX, posY - y * boxHeight, width, lineThickness, 0, color);
        float waveLength = width / numWaveUnitsPerLineStrip;
        float angle = startAngle;
        float *verts = new float[numVerts * 2];
        float gapBwVerts = width / (numVerts - 1);

        for(int vert = 0; vert < numVerts; vert++)
        {
            angle += 360 / (waveLength / gapBwVerts);
            if(angle >= 360) angle -= 360;
            float radian = angle * 3.1415 / 180;
            float offsetY = sin(radian) * amplitude;

            verts[0 + vert * 2] = gapBwVerts * vert;
            verts[1 + vert * 2] = 0.0 + offsetY;
        }

        GraphicsHelpers::DrawLineStrip(posX, posY - lineY * boxHeight, 0, verts, numVerts, lineThickness, color);

        delete []verts;
        verts = 0;
    }
}

void Grid::Update(double dt)
{
    startAngle += angleSpeed * dt;
}

void Grid::Init()
{
    boxWidth = width / numBoxesX;
    boxHeight = height / numBoxesY;

    amplitude = 0.03;//0.05;
    numWaveUnitsPerLineStrip = 1.0;
    numVerts = 50;
    startAngle = 0.0;
    angleSpeed = 60;//45;//30;
}

void Grid::GetPositionWorld(int gridX, int gridY, float* x, float* y)
{
    *x = NAN;
    *y = NAN;

    if(gridX < 0 || gridX >= numBoxesX || gridY < 0 || gridY >= numBoxesY)
        return;

    *x = gridX * boxWidth + boxWidth / 2.0 + posX;
    *y = posY - boxHeight / 2.0 - gridY*boxHeight;
}


void Grid::GetOffsetXY(int gridX, int gridY, float* offX, float* offY)
{
    *offX = 0;
    *offY = 0;

    if(gridX < 0 || gridY < 0 || gridX >= numBoxesX || gridY >= numBoxesY)
        return;

    float waveLengthX = width / numWaveUnitsPerLineStrip;
    float waveLengthY = height / numWaveUnitsPerLineStrip;
    float gapBwVertsX = width / (numVerts - 1);
    float gapBwVertsY = height / (numVerts - 1);
    float vertsPerBoxX = boxWidth / gapBwVertsX;
    float vertsPerBoxY = boxHeight / gapBwVertsY;
    float angleX = startAngle + 360 / (waveLengthX / gapBwVertsX) * (gridX * vertsPerBoxX + vertsPerBoxX / 2);
    float angleY = startAngle + 360 / (waveLengthY / gapBwVertsY) * (gridY * vertsPerBoxY + vertsPerBoxY / 2);

    *offY = sin(angleX * 3.1415 / 180) * amplitude;
    *offX = sin(angleY * 3.1415 / 180) * amplitude;
}

