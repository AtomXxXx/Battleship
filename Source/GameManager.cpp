#include "GameManager.h"
extern GameManager gameManager;

bool GameManager::SelectShip(float x, float y)
{
    bool shipSelected = false;
    for(int i = 0; i < numShips; i++)
    {
        if(board1.IsInsideShip(x, y, i))
        {
            board1.selectedShip = i;

            shipSelected = true;
            break;
        }
    }

    if(!shipSelected)
        return false;

    int yGrid = board1.ships[board1.selectedShip].gridPosY;
    int xGrid = board1.ships[board1.selectedShip].gridPosX;
    int length = board1.ships[board1.selectedShip].length;

    if(!(xGrid < 0 || yGrid < 0))
    {

        if(!board1.ships[board1.selectedShip].isVertical)
        {
            for(; xGrid < board1.ships[board1.selectedShip].gridPosX + length; xGrid++)
                board1.states[xGrid + yGrid * board1.grid.numBoxesX] = Empty;
        }
        else
        {
            for(; yGrid < board1.ships[board1.selectedShip].gridPosY + length; yGrid++)
                board1.states[xGrid + yGrid * board1.grid.numBoxesX] = Empty;
        }
    }
    return true;
}

void GameManager::AIPlaceShips()
{
    int numVerticals = rand() % numShips + 1;
    int shipNum;

    for(int i = 0; i < numVerticals; i++)
    {
        shipNum = rand() % numShips;
        if(board2.ships[shipNum].isVertical == false)
            board2.ShipToggleVerticle(shipNum);
        else
            i--;
    }

    for(int i = 0; i < numShips; i++)
    {
        shipNum = rand() % numShips;
        if(board2.ships[shipNum].gridPosX < 0 || board2.ships[shipNum].gridPosY < 0)
        {

            do
            {
                board2.selectedShip = shipNum;
                board2.ships[shipNum].gridPosX = rand() % board2.grid.numBoxesX;
                board2.ships[shipNum].gridPosY = rand() % board2.grid.numBoxesY;
            }while(!board2.PlaceSelectedShip());

        }
        else
        {
            i--;
            continue;
        }
    }

    for(int y = 0; y < board2.grid.numBoxesY; y++)
    {
        for(int x = 0; x < board2.grid.numBoxesX; x++)
            if(board2.states[x + y * board2.grid.numBoxesX] == Empty)
                printf("0 ");
            else printf("1 ");

        printf("\n");
    }
    printf("\n");
        for(int y = 0; y < board1.grid.numBoxesY; y++)
    {
        for(int x = 0; x < board1.grid.numBoxesX; x++)
            if(board1.states[x + y * board1.grid.numBoxesX] == Empty)
                printf("0 ");
            else printf("1 ");

        printf("\n");
    }
}

void GameManager::HandleInput(float x, float y, bool buttonPressed)
{
    if(buttonPressed)
    {
        switch(gameState)
        {
            case ShipNotSelected:
                if(SelectShip(x, y));
                    gameState = ShipSelected;
                break;

            case ShipSelected:
                if(board1.PlaceSelectedShip())
                {
                    if(board1.AllShipsOnBoard())
                    {
                        AIPlaceShips();
                        gameState = PlayerTurn;
                        break;
                    }
                }
                gameState = ShipNotSelected;
                break;

            case PlayerTurn:
            {
                //if(explosion.playing) break;
                if(gameManager.animationController.IsBlockingAnimationPlaying()) break;
                int gridX, gridY;
                if(board2.GetGridNumber(x, y, &gridX, &gridY))
                {
                    int shipNum = -1;
                    GridState state = board2.Fire(gridX, gridY, shipNum);
                    if(state == Empty)
                    {
                        gameManager.gameState = AITurn;
                        gameManager.board2.cursorPosX = -1;
                        gameManager.board2.cursorPosY = -1;

                        float animX, animY, animWidth, animHeight;
                        board2.grid.GetPositionWorld(gridX, gridY, &animX, &animY);
                        animWidth = board2.grid.boxWidth * 2.0;
                        animHeight = board2.grid.boxHeight * 2.0;
                        gameManager.animationController.InstantiateAnimation(waterSplash, animX, animY, animWidth, animHeight, false, false);
                    }
                    else if(state == ShipPresent)
                    {
                        float animX, animY, animWidth, animHeight;
                        board2.grid.GetPositionWorld(gridX, gridY, &animX, &animY);
                        /*explosion.SetPosition(animX, animY);
                        explosion.SetVertices(board2.grid.boxWidth * 2.0, board2.grid.boxHeight * 2.0);
                        explosion.Play(false);*/
                        animWidth = board2.grid.boxWidth * 2.0;
                        animHeight = board2.grid.boxHeight * 2.0;
                        gameManager.animationController.InstantiateAnimation(explosion, animX, animY, animWidth, animHeight, false, false);

                        if(board2.AllShipsDestroyed())
                        {
                            printf("You Win ~~~\\<(^o^)>/~~~");
                                gameManager.gameState = GameOver;
                                //gameManager.quitGame = true;
                        }
                    }

                }
                break;
            }
            case AITurn:
            {
                break;
            }
            case GameOver:
                gameManager.gameState = Credits;
                break;
            case TitleScreen:
            {
                gameManager.gameState = ShipNotSelected;
                break;
            }
            default:
                break;
        }
    }
    else
    {
        if(gameState == ShipSelected)
        {
            if(!board1.GetGridNumber(x, y, &board1.ships[board1.selectedShip].gridPosX, &board1.ships[board1.selectedShip].gridPosY))
            {
                board1.ships[board1.selectedShip].posX = x;
                board1.ships[board1.selectedShip].posY = y;
            }
        }
        else if(gameState == PlayerTurn)
        {
            board2.GetGridNumber(x, y, &board2.cursorPosX, &board2.cursorPosY);
        }
    }
}

void GameManager::DrawTitle()
{
    GraphicsHelpers::DrawTexturedRectangle(title.textureID, 0.0, 0.5, 10.0, 5.0, 0.0);
    GraphicsHelpers::DrawString(-2.2, -3.0, 0.004, 0.0035, 0.006, 1.0, 1.0, 1.0, "Click to continue");
}

void GameManager::DrawCredits()
{
    GraphicsHelpers::DrawString(-3.0, 0.0, 0.005, 0.0045, 0.007, 1.0, 1.0, 1.0, "Thanks for playing!");
}

void GameManager::InitGame()
{
    quitGame = false;
    numShips = 5; // must be 5

    board1.grid.posX = -6.0f;
    board1.grid.posY = 4.0f;
    board1.grid.width = 5.0f;
    board1.grid.height = 5.0f;
    board1.grid.numBoxesX = 10;
    board1.grid.numBoxesY = 10;
    board1.grid.lineThickness = 0.01f;
    board1.grid.color[0] = 1.0f;
    board1.grid.color[1] = 1.0f;
    board1.grid.color[2] = 1.0f;
    board1.grid.Init();


    board2.grid.posX = 1.0f;
    board2.grid.posY = 4.0f;
    board2.grid.width = 5.0f;
    board2.grid.height = 5.0f;
    board2.grid.numBoxesX = 10;
    board2.grid.numBoxesY = 10;
    board2.grid.lineThickness = 0.01f;
    board2.grid.color[0] = 1.0f;
    board2.grid.color[1] = 1.0f;
    board2.grid.color[2] = 1.0f;
    board2.grid.Init();


    board1.states = new GridState[board1.grid.numBoxesY * board1.grid.numBoxesX];
    board2.states = new GridState[board2.grid.numBoxesY * board2.grid.numBoxesX];

    textures[0].LoadTexture("2.png");
    textures[1].LoadTexture("3a.png");
    textures[2].LoadTexture("3b.png");
    textures[3].LoadTexture("4.png");
    textures[4].LoadTexture("5.png");

    waterTexture.LoadTexture("tex_Water.jpg");
    title.LoadTexture("battleship logo.png");
    selectedTileTexture.LoadTexture("circle_gradient.png");

    explosion.Init("Explosion_003_Tile_8x8_256x256.png", 64, 60, 8, 8);
    waterSplash.Init("WaterExplosion_8x8.png", 64, 60, 8, 8);

    Texture *explosionTex = new Texture;
    explosionTex->LoadTexture("Explosion_003_Tile_8x8_256x256.png");
    animationController.spriteSheets.push_back(explosionTex);

    Texture* waterSplashTex = new Texture;
    waterSplashTex->LoadTexture("WaterExplosion_8x8.png");
    animationController.spriteSheets.push_back(waterSplashTex);

    board1.Init();
    board2.Init();

    //gameState = ShipNotSelected;
    gameState = TitleScreen;
}
