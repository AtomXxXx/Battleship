#pragma once

#include "States.h"
#include "Grid.h"
#include "Ship.h"


class Board
{
public:

    void Init();
    void DrawShips();
    void DrawDestroyedShips();
    void DrawGridStates();
    void DrawCursor();
    void UpdateWaterTexture(double dt);
    void DrawWaterTexture();
    bool AllShipsOnBoard();
    bool AllShipsDestroyed();
    bool PlaceSelectedShip();
    GridState Fire(int x, int y, int& shipNum);
    bool IsShipDestroyed(int shipNum);
    // Checks if a ship is present at the given world position
    bool IsInsideShip(float x, float y, int shipNum);
    bool ShipToggleVerticle(int shipNum);
    // Converts the world position to a grid position
    bool GetGridNumber(float xWorld, float yWorld, int* x, int* y);
    bool GetShipNumOnGrid(int x, int y, int* shipNum);

    Grid grid;
    GridState* states;
    Ship* ships;

    int numShipsDestroyed;
    int selectedShip;

    int cursorPosX;
    int cursorPosY;

    struct WaterAnim
    {
        float u; // Current texture coordinates
        float v;

        float speed; // in Texture units per second
        float dirX; // Normalized vector determines the direction the texture is moving
        float dirY;

        float verts[4][2];
        float uv[4][2];
    };
    WaterAnim waterAnim[2]; // Two water textures moving in different directions to create wave effect
};
