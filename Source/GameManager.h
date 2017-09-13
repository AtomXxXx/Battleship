#pragma once

#include "AI.h"
#include "States.h"
#include "AnimationController.h"
#include "Board.h"
#include "Texture.h"
#include "Animation.h"

struct GameManager
{
public:

    GameManager()
        :
        ai(board1, board2)
    {

    }
    void InitGame();
    void HandleInput(float x, float y, bool buttonPressed);
    bool SelectShip(float x, float y);
    void AIPlaceShips();
    void DrawTitle();
    void DrawCredits();

    GraphicsHelpers graphics;

    AnimationController animationController;
    GameState gameState;
    Board board1;
    Board board2;
    AI ai;

    Texture textures[5];
    Texture waterTexture;
    Texture title;
    Texture selectedTileTexture;

    //Animation explosion;

    AnimationInfo explosion;
    AnimationInfo waterSplash;


    int numShips; // must be 5
    Ship ship;

    bool quitGame;
};
