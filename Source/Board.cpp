#include "Board.h"
#include "GameManager.h"
extern GameManager gameManager;

void Board::Init()
{
    for(int y = 0; y < grid.numBoxesY; y++)
        for(int x = 0; x < grid.numBoxesX; x++)
            states[x + y * grid.numBoxesX] = Empty;
    selectedShip = -1;
    numShipsDestroyed = 0;
    cursorPosX = -1;
    cursorPosY = -1;

    ships = new Ship[gameManager.numShips];

    // Initialize the ships
    int shipGridLength = 5;
    for(int i = 0; i < gameManager.numShips; i++)
    {
        ships[i].gridPosX = -1;
        ships[i].gridPosY = -1;
        ships[i].isVertical = false;
        ships[i].numPartsDestoryed = 0;

        // Only supports 5 ships as of now
        // lengths according to the rules must be 5, 4, 3, 3, 2
        if(i == 3) shipGridLength++;
        ships[i].length = shipGridLength--;

        ships[i].width = grid.boxWidth * ships[i].length;
        ships[i].height = grid.boxHeight;

        ships[i].defaultPosX = grid.posX + grid.boxWidth * 2 - grid.boxWidth / 2;
        ships[i].defaultPosY = (grid.posY - grid.height) - ((ships[i].height) * i) - grid.boxHeight / 2 - (i+1) * 0.15;

        ships[i].posX = ships[i].defaultPosX;
        ships[i].posY = ships[i].defaultPosY;

        switch(ships[i].length)
        {
        case 2:
            ships[i].textureID = gameManager.textures[0].textureID;
            break;
        case 3:
            if(i == 3)
                ships[i].textureID = gameManager.textures[1].textureID;
            else
                ships[i].textureID = gameManager.textures[2].textureID;
            break;
        case 4:
            ships[i].textureID = gameManager.textures[3].textureID;
            break;
        case 5:
            ships[i].textureID = gameManager.textures[4].textureID;
            break;
        }

    }

    /*for(int y = 0; y < grid.numBoxesY; y++)
    {
        for(int x = 0; x < grid.numBoxesX; x++)
        {
            float animX, animY;
            grid.GetPositionWorld(x, y, &animX, &animY);
            gameManager.animationController.InstantiateAnimation(gameManager.oceanWater, animX, animY, grid.boxWidth, grid.boxHeight, true, false);
        }
    }*/

    // Initialize water 'animation'
    for(int i = 0; i < 2; i++)
    {
        waterAnim[i].dirX = 0.0;
        waterAnim[i].dirY = 0.0;
        waterAnim[i].speed = 0;
        waterAnim[i].u = 0.0;
        waterAnim[i].v = 0.0;

        waterAnim[i].uv[0][0] = 0.0;
        waterAnim[i].uv[0][1] = 0.0;

        waterAnim[i].uv[1][0] = 1.0;
        waterAnim[i].uv[1][1] = 0.0;

        waterAnim[i].uv[2][0] = 1.0;
        waterAnim[i].uv[2][1] = 1.0;

        waterAnim[i].uv[3][0] = 0.0;
        waterAnim[i].uv[3][1] = 1.0;

        waterAnim[i].verts[0][0] = 0.0;
        waterAnim[i].verts[0][1] = 0.0;

        waterAnim[i].verts[1][0] = grid.width;
        waterAnim[i].verts[1][1] = 0.0;

        waterAnim[i].verts[2][0] = grid.width;
        waterAnim[i].verts[2][1] = -grid.height;

        waterAnim[i].verts[3][0] = 0.0;
        waterAnim[i].verts[3][1] = -grid.height;
    }
    // Avoids a weird artifact when the two textures are perfectly aligned
    waterAnim[1].uv[0][0] += 0.5;
    waterAnim[1].uv[1][0] += 0.5;
    waterAnim[1].uv[2][0] += 0.5;
    waterAnim[1].uv[3][0] += 0.5;

    waterAnim[0].dirX = 0.70710678118; //Normalized vector (1, 1)
    waterAnim[0].dirY = 0.70710678118;
    waterAnim[0].speed = 0.05;
    waterAnim[1].dirX = -0.70710678118; // Normalized vector (-1, -1)
    waterAnim[1].dirY = -0.70710678118;
    waterAnim[1].speed = 0.08;
}

void Board::DrawCursor()
{
    if(cursorPosX < 0 || cursorPosY < 0 || cursorPosX >= grid.numBoxesX || cursorPosY >= grid.numBoxesY)
        return;

    float width = grid.boxWidth * 0.8;
    float height = grid.boxHeight * 0.8;
    float smallLineWidth = width / 3;
    float smallLineHeight = height / 3;

    float lineThickness = GraphicsHelpers::WorldToPixels(0.008);

    float worldPosX, worldPosY;
    grid.GetPositionWorld(cursorPosX, cursorPosY, &worldPosX, &worldPosY);
    float color[3] = {1.0, 0.0, 0.0};

    // Should look like this
    // --  --
    // |    |
    //
    // |    |
    // --  --

    // Top Left Horizontal
    GraphicsHelpers::DrawLine(worldPosX - width / 2, worldPosY + height / 2, smallLineWidth, lineThickness, 0, color);
    // Top Right Horizontal
    GraphicsHelpers::DrawLine(worldPosX + width / 2, worldPosY + height / 2, smallLineWidth, lineThickness, 180, color);
    // Bottom Left Horizontal
    GraphicsHelpers::DrawLine(worldPosX - width / 2, worldPosY - height / 2, smallLineWidth, lineThickness, 0, color);
    // Bottom Right Horizontal
    GraphicsHelpers::DrawLine(worldPosX + width / 2, worldPosY - height / 2, smallLineWidth, lineThickness, 180, color);

    // Top Left Vertical
    GraphicsHelpers::DrawLine(worldPosX - width / 2, worldPosY + height / 2, smallLineHeight, lineThickness, -90, color);
    // Top Right Vertical
    GraphicsHelpers::DrawLine(worldPosX + width / 2, worldPosY + height / 2, smallLineHeight, lineThickness, -90, color);
    // Bottom Left Vertical
    GraphicsHelpers::DrawLine(worldPosX - width / 2, worldPosY - height / 2, smallLineHeight, lineThickness, 90, color);
    // Bottom Right Vertical
    GraphicsHelpers::DrawLine(worldPosX + width / 2, worldPosY - height / 2, smallLineHeight, lineThickness, 90, color);
}

void Board::UpdateWaterTexture(double dt)
{
    for(int i = 0; i < 2; i++)
    {
        for(int texPoint = 0; texPoint < 4; texPoint++)
        {
            waterAnim[i].uv[texPoint][0] += waterAnim[i].dirX * waterAnim[i].speed * dt;
            waterAnim[i].uv[texPoint][1] += waterAnim[i].dirY * waterAnim[i].speed * dt;

        }
        // Bring back the uv values to within 1.0 if the values are too large
        float offsetValX = 0.0;
        float offsetValY = 0.0;

        if(waterAnim[i].uv[0][0] > 1.0)
            offsetValX = -1.0;
        else if(waterAnim[i].uv[0][0] < -1.0)
            offsetValX = 1.0;

        if(waterAnim[i].uv[0][1] > 1.0)
            offsetValY = -1.0;
        else if(waterAnim[i].uv[0][1] < -1.0)
            offsetValY = 1.0;

        for(int texPoint = 0; texPoint < 4; texPoint++)
        {
            waterAnim[i].uv[texPoint][0] += offsetValX;
            waterAnim[i].uv[texPoint][1] += offsetValY;
        }
    }
}
void Board::DrawWaterTexture()
{
    GraphicsHelpers::DrawTexturedRectangle(waterAnim[0].verts, waterAnim[0].uv, gameManager.waterTexture.textureID, grid.posX, grid.posY, 0.0, 1.0);
    GraphicsHelpers::DrawTexturedRectangle(waterAnim[1].verts, waterAnim[1].uv, gameManager.waterTexture.textureID, grid.posX, grid.posY, 0.0, 0.5);
}

bool Board::IsShipDestroyed(int shipNum)
{
    if(shipNum < 0 || shipNum >= gameManager.numShips)
    {
        return false;
    }
    else
        return (ships[shipNum].numPartsDestoryed == ships[shipNum].length);
}

bool Board::AllShipsDestroyed()
{
    if(numShipsDestroyed == gameManager.numShips)
        return true;
    return false;
}

//Fire returns ship number only if its destroyed
GridState Board::Fire(int x, int y, int& shipNum)
{
    shipNum = -1;
    if(x < 0 || y < 0 || x >= grid.numBoxesX || y >= grid.numBoxesY)
        return Invalid;

    if(states[x + y * grid.numBoxesX] == Empty)
    {
        states[x + y * grid.numBoxesX] = SelectedEmpty;
        return Empty;
    }
    else if(GetShipNumOnGrid(x, y, &shipNum))
    {
        states[x + y * grid.numBoxesX] = ShipDestroyed;
        ships[shipNum].numPartsDestoryed++;
        if(ships[shipNum].numPartsDestoryed == ships[shipNum].length)
        {
            numShipsDestroyed++;
            return ShipPresent;

        }
        //ship not destroyed, dont return shipNum
        shipNum = -1;
        return ShipPresent;
    }
    else
    {
        return Selected;
    }
}

bool Board::GetGridNumber(float xWorld, float yWorld, int* x, int* y)
{
    float xBoardSpace = -(grid.posX - xWorld);
    float yBoardSpace = grid.posY - yWorld;

    float boxNumX = xBoardSpace / grid.boxWidth;
    float boxNumY = yBoardSpace / grid.boxHeight;

    if(boxNumX < 0 || boxNumY < 0 || boxNumX >= grid.numBoxesX || boxNumY >= grid.numBoxesY)
    {
        *x = -1;
        *y = -1;
        return false;
    }

    *x = boxNumX;
    *y = boxNumY;

    return true;
}

bool Board::ShipToggleVerticle(int shipNum)
{
    ships[shipNum].isVertical = !ships[shipNum].isVertical;

    if(ships[shipNum].isVertical)
    {
        ships[shipNum].width = grid.boxWidth;
        ships[shipNum].height = grid.boxHeight * ships[shipNum].length;
    }
    else
    {
        ships[shipNum].width = grid.boxWidth * ships[shipNum].length;
        ships[shipNum].height = grid.boxHeight;
    }
    return true;
}

bool Board::AllShipsOnBoard()
{
    for(int i = 0; i < gameManager.numShips; i++)
        if(ships[i].gridPosX < 0 || ships[i].gridPosY < 0)
            return false;
    return true;
}

bool Board::IsInsideShip(float x, float y, int shipNum)
{
    float topLeftX = ships[shipNum].posX - grid.boxWidth / 2;
    float topLeftY = ships[shipNum].posY + grid.boxHeight / 2;

    if(x >= topLeftX && x <= topLeftX + ships[shipNum].width &&
        y <= topLeftY && y >= topLeftY - ships[shipNum].height)
        return true;

    return false;
}

bool Board::PlaceSelectedShip()
{
    if(selectedShip < 0 || selectedShip >= gameManager.numShips)
        return false;

    bool canPlace = true;
    int length = ships[selectedShip].length;
    int y = ships[selectedShip].gridPosY;
    int x = ships[selectedShip].gridPosX;

    if(x < 0 || y < 0)
    {
        ships[selectedShip].posX = ships[selectedShip].defaultPosX;
        ships[selectedShip].posY = ships[selectedShip].defaultPosY;
        ships[selectedShip].gridPosX = -1;
        ships[selectedShip].gridPosY = -1;
        if(ships[selectedShip].isVertical)
            ShipToggleVerticle(selectedShip);
        selectedShip = -1;
        return false;
    }

    if(!ships[selectedShip].isVertical)
    {
        for(; (x < grid.numBoxesX && x < (ships[selectedShip].gridPosX + length)); x++)
        {
            if(states[x + y * grid.numBoxesX] != Empty)
            {
                canPlace = false;
                break;
            }
        }
        if(canPlace == true && x < (ships[selectedShip].gridPosX + length))
        {
            canPlace = false;
        }
    }
    else
    {
        for(; (y < grid.numBoxesY && y < (ships[selectedShip].gridPosY + length)); y++)
        {
            if(states[x + y * grid.numBoxesX] != Empty)
            {
                canPlace = false;
                break;
            }
        }
        if(canPlace == true && y < (ships[selectedShip].gridPosY + length))
        {
            canPlace = false;
        }
    }


    if(!canPlace)
    {
        ships[selectedShip].posX = ships[selectedShip].defaultPosX;
        ships[selectedShip].posY = ships[selectedShip].defaultPosY;
        ships[selectedShip].gridPosX = -1;
        ships[selectedShip].gridPosY = -1;
        if(ships[selectedShip].isVertical)
            ShipToggleVerticle(selectedShip);
        selectedShip = -1;
        return false;
    }

    y = ships[selectedShip].gridPosY;
    x = ships[selectedShip].gridPosX;

    if(!ships[selectedShip].isVertical)
    {
        for(; x < ships[selectedShip].gridPosX + length; x++)
            states[x + y * grid.numBoxesX] = ShipPresent;
    }
    else
    {
        for(; y < ships[selectedShip].gridPosY + length; y++)
            states[x + y * grid.numBoxesX] = ShipPresent;
    }
    selectedShip = -1;
    return true;
}

bool Board::GetShipNumOnGrid(int x, int y, int* shipNum)
{
    *shipNum = -1;
    if(x > grid.numBoxesX || x < 0 || y > grid.numBoxesY || y < 0)
        return false;

    if(states[x + y * grid.numBoxesX] != ShipPresent)
        return false;

    for(int i = 0; i < gameManager.numShips; i++)
    {
        if(ships[i].isVertical)
        {
            if(ships[i].gridPosX != x)
                continue;

            if(y >= ships[i].gridPosY && y < (ships[i].gridPosY + ships[i].length))
            {
                *shipNum = i;
                return true;
            }
            else continue;
        }
        else
        {
            if(ships[i].gridPosY != y)
                continue;

            if(x >= ships[i].gridPosX && x < (ships[i].gridPosX + ships[i].length))
            {
                *shipNum = i;
                return true;
            }
            else continue;
        }
    }

    return false;
}



void Board::DrawShips()
{
    for(int i = 0; i < gameManager.numShips; i++)
    {
        ships[i].Draw(grid, i == selectedShip);
    }
}

void Board::DrawDestroyedShips()
{
    for(int i = 0; i < gameManager.numShips; i++)
    {
        if(ships[i].numPartsDestoryed == ships[i].length)
        {
            ships[i].Draw(grid, false);
        }
    }
}

void Board::DrawGridStates()
{
    for(int y = 0; y < grid.numBoxesY; y++)
    {
        for(int x = 0; x < grid.numBoxesX; x++)
        {
            switch(states[x + y * grid.numBoxesX])
            {
                case SelectedEmpty:
                {
                    // Draw green (or red??) square
                    //float color[3] = {0.0, 1.0, 0.0};
                    //float color[3] = {0.0, 0.0, 0.8};
                    //DrawRectangle(grid.posX + grid.boxWidth * x + grid.boxWidth / 2.0, grid.posY - grid.boxHeight * y - grid.boxHeight / 2.0, grid.boxWidth / 2, grid.boxHeight / 2, 0.0f, color);
                    GraphicsHelpers::DrawTexturedRectangle(gameManager.selectedTileTexture.textureID, grid.posX + grid.boxWidth * x + grid.boxWidth / 2.0,
                                          grid.posY - grid.boxHeight * y - grid.boxHeight / 2.0, grid.boxWidth * 1.2, grid.boxHeight * 1.2, 0.0, 0.0, 0.0, 1.0);
                    break;
                }

                case ShipDestroyed:
                {
                    //float color[3] = {1.0, 0.0, 0.0};
                    //DrawRectangle(grid.posX + grid.boxWidth * x + grid.boxWidth / 2.0, grid.posY - grid.boxHeight * y - grid.boxHeight / 2.0, grid.boxWidth / 2, grid.boxHeight / 2, 0.0f, color);
                    GraphicsHelpers::DrawTexturedRectangle(gameManager.selectedTileTexture.textureID, grid.posX + grid.boxWidth * x + grid.boxWidth / 2.0,
                                          grid.posY - grid.boxHeight * y - grid.boxHeight / 2.0, grid.boxWidth * 1.2, grid.boxHeight * 1.2, 0.0, 1.0, 0.0, 0.0);
                    break;
                }
                default:
                    break;
            }
        }
    }
}
