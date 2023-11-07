#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obstacles.h"
#include "func.h"

#define WIDTH  800
#define HEIGHT 900
#define PlayerWidth 60
#define PlayerHeight 60
#define LimitFps 16
#define MAX_ACTIVE_OBSTACLES 6
#define PlayerXSpawnPoint 15




Obstacle predefinedObstacles[] = {
    {{{40, HEIGHT-60, 30, 60},{40, HEIGHT-60, 60, 30}} //L a l'envers
    },
    {{{100, HEIGHT-60, 60, 30},{115, HEIGHT-60, 30, 60}} // T
    },
    {{{160, HEIGHT-60, 60, 30},{175, HEIGHT-60-45, 30, 60}} // T a l'envers
    },
    {{220, HEIGHT-60,60,60} // carre 1x1
    },
    {{280,HEIGHT-60,60,30} // barre horizontale
    },
    {{340,HEIGHT-60,30,60} // barre Verticale
    }
};

int main(int argc, char** argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_ExitWithError("Init SDL");
    }

    if(SDL_CreateWindowAndRenderer(WIDTH,HEIGHT, 0, &win, &ren) != 0){
        SDL_ExitWithError("Window and renderer creation");
    }


/*-------------------------------------------------------------------------*/
            //VARIABLE Gameplay
/*-------------------------------------------------------------------------*/
    int wallY,wallH,*ptrWallY,*ptrWallH;
    wallY = HEIGHT/4;
    wallH = HEIGHT-HEIGHT/4;
    ptrWallY = &wallY;
    ptrWallH = &wallH;

    int playerY,playerX, *ptrPlayerX,*ptrPlayerY;
    playerY = HEIGHT/4 - 62;
    playerX = PlayerXSpawnPoint;
    ptrPlayerX = &playerX;
    ptrPlayerY = &playerY;

    SDL_bool running = SDL_TRUE;
    int playing = 0;
    unsigned int frame = 0;
    int i = rng(5);
    int x_base = rngXPos();

    ObstaclesNode* obstaclesListStart = NULL;
    ObstaclesNode* obstaclesListEnd = NULL;

/*-------------------------------------------------------------------------*/
            //main loop
/*-------------------------------------------------------------------------*/
    drawPlayer(ren,*ptrPlayerX,*ptrPlayerY);
    drawWalls(ren,ptrWallY,ptrWallH);
    frame = SDL_GetTicks() + LimitFps;

    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[i]);
    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[i+1]);
    while (running)
    {   SDL_Event event;
        limit_FPS(frame);

        if(playing == 1){
            
            
            drawObstacleList(ren,obstaclesListStart);
            eraseGamingField(ren);
            drawPlayer(ren,*ptrPlayerX,*ptrPlayerY);
            wallY -= 1;
            wallH += 1;
            drawWalls(ren,ptrWallY,ptrWallH);
            upObstacleList(obstaclesListStart);
            drawObstacleList(ren,obstaclesListStart);

        }

        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if(playing == 0){
                                playing = 1;
                                StartingGame(ren,ptrPlayerX,ptrPlayerY);
                                
                                continue;}
                        case SDLK_a:
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_d:
                            if(playing == 1){
                                moveRight(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_ESCAPE:
                            if(playing == 1){
                                playing = 2;
                            }
                            else if (playing == 2){
                                playing = 1;
                            }
                            continue;

                        default:
                            continue;

                    }
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
        
        
        frame = SDL_GetTicks() + LimitFps;
        SDL_RenderPresent(ren);
    }
     

    
/*-------------------------------------------------------------------------*/
            //Close all
/*-------------------------------------------------------------------------*/
    removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}




