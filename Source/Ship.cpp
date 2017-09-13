#include "Ship.h"

void Ship::Draw(Grid& grid, bool selected)
{
    float uv[4][2] = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};

    if(!(gridPosX < 0 || gridPosY < 0))
    {
        posX = grid.posX + gridPosX * grid.boxWidth + grid.boxWidth / 2.0;
        posY = grid.posY - (gridPosY * grid.boxHeight + grid.boxHeight / 2.0);
    }

    float r = 1.0, g = 1.0, b = 1.0, a = 1.0;
    if(length == numPartsDestoryed)
    {
        r = 0.5;
        g = 0.2;
        b = 0.2;
        a = 0.7;
    }
    else if (numPartsDestoryed != 0)
    {
        a = 0.7;
    }

    if(!isVertical)
    {
        // Find the offset value so that the ship moves with the wavy lines
        float offX, offY;
        grid.GetOffsetXY(gridPosX + ((length-1) / 2 + 0.5), gridPosY, &offX, &offY);
        float vertices[4][2] = { {-grid.boxWidth / 2, -grid.boxHeight / 2},
                            {-grid.boxWidth / 2, grid.boxHeight / 2},
                            {width - grid.boxWidth / 2, grid.boxHeight / 2},
                            {width - grid.boxWidth / 2, -grid.boxHeight / 2} };

        // posY - offY gives a nicer effect than +offY
        GraphicsHelpers::DrawTexturedRectangle(vertices, uv, textureID, posX + offX, posY + -offY, 0, r, g, b, a);
    }
    else
    {
        float offX, offY;
        grid.GetOffsetXY(gridPosX, gridPosY + ((length-1)/ 2 + 0.5), &offX, &offY);
        float vertices[4][2] = { {-grid.boxHeight / 2, -grid.boxWidth / 2},
                            {-grid.boxHeight / 2, grid.boxWidth / 2},
                            {height - grid.boxHeight / 2, grid.boxWidth / 2},
                            {height - grid.boxHeight / 2, -grid.boxWidth / 2} };

        GraphicsHelpers::DrawTexturedRectangle(vertices, uv, textureID, posX + offX, posY + -offY, -90, r, g, b, a);
    }

}
