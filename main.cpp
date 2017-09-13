
#include <time.h>
#include <GL/freeglut.h>
#include <GL/gl.h>


#include "Source/GraphicsHelpers.h"
#include "Source/GameManager.h"


GameManager gameManager;


void InitGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-GraphicsHelpers::ASPECT_WIDTH / 2.0, GraphicsHelpers::ASPECT_WIDTH / 2.0, -GraphicsHelpers::ASPECT_HEIGHT / 2.0, GraphicsHelpers::ASPECT_HEIGHT / 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void Update(double dt)
{
    //angle += speed * dt;
    //if(angle >= 360) angle -= 360;

    gameManager.animationController.Update(dt);
    gameManager.board1.UpdateWaterTexture(dt);
    gameManager.board2.UpdateWaterTexture(dt);
    gameManager.board1.grid.Update(dt);
    gameManager.board2.grid.Update(dt);

    if(gameManager.gameState == AITurn)
    {
        // wait for X secs
        if(gameManager.ai.timer <= 1.75)
        {
            gameManager.ai.timer += dt;
            return;
        }
        //if(gameManager.explosion.playing) return;
        if(gameManager.animationController.IsBlockingAnimationPlaying()) return;
        gameManager.ai.timer = 0.0;

        int x, y;
        GridState state = gameManager.ai.Play(&x, &y);

        if(state == Empty)
        {
            gameManager.gameState = PlayerTurn;
            float animX, animY, animWidth, animHeight;
            gameManager.board1.grid.GetPositionWorld(x, y, &animX, &animY);
            animWidth = gameManager.board1.grid.boxWidth * 2.0;
            animHeight = gameManager.board1.grid.boxHeight * 2.0;
            gameManager.animationController.InstantiateAnimation(gameManager.waterSplash, animX, animY, animWidth, animHeight, false, false);
        }
        else if(state == ShipPresent)
        {
            float animX, animY, animWidth, animHeight;
            gameManager.board1.grid.GetPositionWorld(x, y, &animX, &animY);
            /*gameManager.explosion.SetPosition(animX, animY);
            gameManager.explosion.SetVertices(gameManager.board1.grid.boxWidth * 2.0, gameManager.board1.grid.boxHeight * 2.0);
            gameManager.explosion.Play(false);*/

            animWidth = gameManager.board1.grid.boxWidth * 2.0;
            animHeight = gameManager.board1.grid.boxHeight * 2.0;
            gameManager.animationController.InstantiateAnimation(gameManager.explosion, animX, animY, animWidth, animHeight, false, true);

            if(gameManager.board1.AllShipsDestroyed())
            {
                printf("Computer Wins <(^^)>\n");
                gameManager.gameState = GameOver;
            }
        }
    }
}



void RenderFunction()
{
    //glClearColor(0.3, 0.3, 1.0, 0.0);
    glClearColor(0.3, 0.3, 0.3, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if(gameManager.gameState == TitleScreen)
    {
        gameManager.DrawTitle();
        return;
    }
    else if(gameManager.gameState == ShipNotSelected || gameManager.gameState == ShipSelected)
    {
        GraphicsHelpers::DrawString(-1.5, -2.0, 0.003, 0.003, 0.007, 1.0, 1.0, 1.0, "Place your ships");
        GraphicsHelpers::DrawString(-1.5, -3.0, 0.0025, 0.0025, 0.006, 1.0, 1.0, 1.0, "Right click to rotate");
    }
    else if(gameManager.gameState == PlayerTurn)
    {
        GraphicsHelpers::DrawString(gameManager.board2.grid.posX + 0.75, gameManager.board2.grid.posY - gameManager.board2.grid.height - 1.0, 0.005, 0.005, 0.007, 1.0, 1.0, 1.0, "Player Turn");
        GraphicsHelpers::DrawString(gameManager.board1.grid.posX + 0.15, gameManager.board1.grid.posY - gameManager.board1.grid.height - 1.0, 0.005, 0.005, 0.005, 0.5, 0.5, 0.5, "Computer Turn");
    }
    else if(gameManager.gameState == AITurn)
    {
        GraphicsHelpers::DrawString(gameManager.board1.grid.posX + 0.15, gameManager.board1.grid.posY - gameManager.board1.grid.height - 1.0, 0.005, 0.005, 0.007, 1.0, 1.0, 1.0, "Computer Turn");
        GraphicsHelpers::DrawString(gameManager.board2.grid.posX + 0.75, gameManager.board2.grid.posY - gameManager.board2.grid.height - 1.0, 0.005, 0.005, 0.005, 0.5, 0.5, 0.5, "Player Turn");
    }
    else if(gameManager.gameState == GameOver)
    {
        if(gameManager.board1.numShipsDestroyed == gameManager.numShips)
        {
            GraphicsHelpers::DrawString(-2.4, gameManager.board1.grid.posY - gameManager.board1.grid.height - 1.5, 0.005, 0.005, 0.007, 1.0, 1.0, 1.0, "Computer Wins!");
        }
        else
        {
            GraphicsHelpers::DrawString(-1.4, gameManager.board1.grid.posY - gameManager.board1.grid.height - 1.5, 0.005, 0.005, 0.007, 1.0, 1.0, 1.0, "You Win!");
        }
    }
    else if(gameManager.gameState == Credits)
    {
        gameManager.DrawCredits();
        return;
    }

    gameManager.board1.DrawWaterTexture();
    gameManager.board2.DrawWaterTexture();

    gameManager.board1.grid.DrawGrid();
    gameManager.board2.grid.DrawGrid();
    gameManager.board1.DrawGridStates();
    gameManager.board2.DrawGridStates();
    gameManager.board1.DrawShips();
    if(gameManager.gameState != GameOver)
        gameManager.board2.DrawDestroyedShips();
    else
        gameManager.board2.DrawShips();

    if(gameManager.gameState == PlayerTurn)
        gameManager.board2.DrawCursor();

    gameManager.animationController.Draw();

    //float color[3] = {1.0, 1.0, 1.0};
    //GraphicsHelpers::DrawLine(0.0f, 0.0f, 4.0f, 0.01f, angle, color);
}


void MouseEvent(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float xMeters, yMeters;
        if(GraphicsHelpers::PixelsToWorld(x, y, &xMeters, &yMeters))
            gameManager.HandleInput(xMeters, yMeters, true);
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if(gameManager.gameState == ShipSelected)
            gameManager.board1.ShipToggleVerticle(gameManager.board1.selectedShip);
    }
}

void MouseMotionEvent(int x, int y)
{
    float xMeters, yMeters;
    if(GraphicsHelpers::PixelsToWorld(x, y, &xMeters, &yMeters))
        gameManager.HandleInput(xMeters, yMeters, false);
}

int main(int argc, char** argv)
{
    putenv( (char *) "__GL_SYNC_TO_VBLANK=1" );
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1024, 576);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Battleship");
    glutReshapeFunc(GraphicsHelpers::ResizeScreen);
    glutMouseFunc(MouseEvent);
    glutMotionFunc(MouseMotionEvent);
    glutPassiveMotionFunc(MouseMotionEvent);
    InitGL();
    srand(time(NULL));

    gameManager.InitGame();

    timespec oldTime;
    long int dtNano = 0;
    // Get high resolution time
    clock_gettime(CLOCK_REALTIME, &oldTime);
    while(!gameManager.quitGame)
    {
        timespec newTime;
        clock_gettime(CLOCK_REALTIME, &newTime);

        // newTime.tv_nsec (long int) resets to 0 around every second
        // This makes oldTime greater than newTime.
        if(newTime.tv_nsec > oldTime.tv_nsec)
            dtNano = newTime.tv_nsec - oldTime.tv_nsec;
        oldTime = newTime;


        glutMainLoopEvent();
        Update(dtNano/(double)(1000.0 * 1000.0 * 1000.0));
        RenderFunction();

        glutSwapBuffers();
    }

    //glutMainLoop();
    return 0;
}
