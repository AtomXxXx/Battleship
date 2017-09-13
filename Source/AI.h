
#pragma once
#include "States.h"
#include "Board.h"

class AI
{
public:

    enum AIState
    {
        FoundShip,
        FindingShip,
        DestroyingShip
    };

    enum Direction
    {
        Left,
        Right,
        Up,
        Down,
        InvalidDirection
    };

    AI(Board& toFireBoard, Board& other)
    : board(toFireBoard),
      aiBoard(other)
    {
        prevX = -1;
        prevY = -1;
        firstHitPosX = -1;
        firstHitPosY = -1;
        prevHitShipNum = -1;
        firingDirection = Left; // start with left
        state = FindingShip;
        prevState = Invalid;
        timer = 0.0;
    }
    // Fills x and y with the played grid position and returns the gridstate at that position
    GridState Play(int *x, int *y);

    int prevX;
    int prevY;

    int firstHitPosX;
    int firstHitPosY;
    Direction firingDirection;

    int prevHitShipNum;

    GridState prevState;
    AIState state;
    double timer;
    Board& board;
    Board& aiBoard;
};
