#include "AI.h"

GridState AI::Play(int *gridX, int *gridY)
{
    // Spaghetti code!! Need to re-factor before I forget the logic.
    int x, y, shipNum = -1;
    GridState gridState;
    bool turnOver = false;

    while(!turnOver)
    {

        switch(state)
        {
            case FindingShip:
            {
                do
                {
                    x = rand() % board.grid.numBoxesX;
                    y = rand() % board.grid.numBoxesY;
                    gridState = board.Fire(x, y, shipNum);
                }while(gridState == Selected);

                turnOver = true;
                if(gridState == ShipPresent)
                {
                    if(shipNum >= 0)
                    {
                        break;
                    }
                    state = FoundShip;
                    firstHitPosX = x;
                    firstHitPosY = y;
                }
                break;
            }

            case FoundShip:
            {
                if(firingDirection == Left)
                {
                    gridState = board.Fire(x = firstHitPosX - 1, y = firstHitPosY, shipNum);
                    if(gridState == Empty)
                    {
                        firingDirection = Right;
                        turnOver = true;
                        break;
                    }
                    else if(gridState == Invalid || gridState == Selected)
                    {
                        firingDirection = Right;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        state = DestroyingShip;
                        break;
                    }
                }

                if(firingDirection == Right)
                {
                    gridState = board.Fire(x = firstHitPosX + 1, y = firstHitPosY, shipNum);
                    if(gridState == Empty)
                    {
                        firingDirection = Up;
                        turnOver = true;
                        break;
                    }
                    else if(gridState == Invalid || gridState == Selected)
                    {
                        firingDirection = Up;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        state = DestroyingShip;
                        break;
                    }
                }
                if(firingDirection == Up)
                {
                    gridState = board.Fire(x = firstHitPosX, y = firstHitPosY - 1, shipNum);
                    if(gridState == Empty)
                    {
                        firingDirection = Down;
                        turnOver = true;
                        break;
                    }
                    else if(gridState == Invalid || gridState == Selected)
                    {
                        firingDirection = Down;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        state = DestroyingShip;
                        break;
                    }
                }
                if(firingDirection == Down)
                {
                    gridState = board.Fire(x = firstHitPosX, y = firstHitPosY + 1, shipNum);
                    if(gridState == Empty)
                    {
                        turnOver = true;
                        firingDirection = Left;
                        break;
                    }
                    else if(gridState == Invalid || gridState == Selected)
                    {
                        firingDirection = Left;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        state = DestroyingShip;
                        break;
                    }
                }
                break;
            }
            case DestroyingShip:
            {
                if(firingDirection == Left)
                {
                    gridState = board.Fire(x = prevX - 1, y = prevY, shipNum);
                    if(gridState == Empty)
                    {
                        turnOver = true;
                        state = FoundShip;
                        firingDirection = Right;
                        break;
                    }
                    else if(gridState == Selected || gridState == Invalid)
                    {
                        state = FoundShip;
                        firingDirection = Right;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        break;
                    }
                }

                if(firingDirection == Right)
                {
                    gridState = board.Fire(x = prevX + 1, y = prevY, shipNum);
                    if(gridState == Empty)
                    {
                        turnOver = true;
                        state = FoundShip;
                        firingDirection = Left;
                        break;
                    }
                    else if(gridState == Selected || gridState == Invalid)
                    {
                        state = FoundShip;
                        firingDirection = Left;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        break;
                    }
                }

                if(firingDirection == Up)
                {
                    gridState = board.Fire(x = prevX, y = prevY - 1, shipNum);
                    if(gridState == Empty)
                    {
                        turnOver = true;
                        state = FoundShip;
                        firingDirection = Down;
                        break;
                    }
                    else if(gridState == Selected || gridState == Invalid)
                    {
                        state = FoundShip;
                        firingDirection = Down;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        break;
                    }
                }
                if(firingDirection == Down)
                {
                    gridState = board.Fire(x = prevX, y = prevY + 1, shipNum);
                    if(gridState == Empty)
                    {
                        turnOver = true;
                        state = FoundShip;
                        firingDirection = Up;
                        break;
                    }
                    else if(gridState == Selected || gridState == Invalid)
                    {
                        state = FoundShip;
                        firingDirection = Up;
                    }
                    else if(gridState == ShipPresent)
                    {
                        turnOver = true;
                        if(shipNum >= 0)
                        {
                            state = FindingShip;
                            shipNum = -1;
                            break;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    AI::prevX = x;
    AI::prevY = y;
    prevState = gridState;

    *gridX = x;
    *gridY = y;
    return gridState;
}
