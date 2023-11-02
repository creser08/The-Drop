#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH  600
#define HEIGHT 900
#define PlayerWidth 60
#define PlayerHeight 60
#define LimitFps 33


void SDL_ExitWithError(const char *errorMsg);
void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg);

void StartingGame(SDL_Renderer *ren, int *playerX, int *playerY);
void limit_FPS(unsigned int limit);

void drawPlayer(SDL_Renderer *ren, int playerX, int playerY);
void erasePlayer(SDL_Renderer *ren, int playerX, int playerY);
void moveRight(SDL_Renderer *ren,int *playerX, int *playerY);
void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY);

void drawWalls(SDL_Renderer *ren, int wallY, int wallH);
void eraseWalls(SDL_Renderer *ren, int wallY, int wallH);
void scrollWalls(SDL_Renderer *ren, int *wallY, int *wallH);

int main(int argc, char** argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
 
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_ExitWithError("Init SDL");
    }

    if(SDL_CreateWindowAndRenderer(WIDTH,HEIGHT, 0, &win, &ren) != 0){
        SDL_ExitWithError("Window and renderer creation");
    }


/*-------------------------------------------------------------------------*/
            //VARIABLE Gameplay
/*-------------------------------------------------------------------------*/
    int *wy,*wh;
    wy = (int *)malloc(sizeof(int));
    wh = (int *)malloc(sizeof(int));

    if (wy != NULL && wh != NULL){
        *wy = HEIGHT/4;
        *wh = HEIGHT-HEIGHT/4;
    }else {
        SDL_ExitWithError("Malloc Wall");
    }

    int *py, *px;
    py = (int *)malloc(sizeof(int));
    px = (int *)malloc(sizeof(int));
    if (py != NULL && px != NULL) {
        *py = HEIGHT/4 - 62; // Attribuer une valeur de base à py
        *px = 15; // Attribuer une valeur de base à px
    } else {
        SDL_ExitWithError("Malloc Player");
    }

/*-------------------------------------------------------------------------*/
            //main loop
/*-------------------------------------------------------------------------*/

    SDL_bool running = SDL_TRUE;
    int playing;
    playing = 0;
    drawPlayer(ren,*px,*py);
    drawWalls(ren,*wy,*wh);
    unsigned int frame = 0;
    frame = SDL_GetTicks() + LimitFps;
    while (running)
    {   
        if (playing == 1){
            limit_FPS(frame);
            eraseWalls(ren,*wy,*wh);
            *wy -= 1;
            *wh += 1;
            drawWalls(ren,*wy,*wh);
            frame = SDL_GetTicks() + LimitFps;
        }    
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if(playing == 0){
                                playing = 1;
                                StartingGame(ren,px,py);
                                continue;}
                        case SDLK_a:
                            if(playing == 1){
                                moveLeft(ren,px,py);
                                continue;
                            }
                        case SDLK_d:
                            if(playing == 1){
                                moveRight(ren,px,py);
                                continue;
                            }
                        case SDLK_ESCAPE:
                            if(playing == 1){
                                playing = 2;
                            }
                            else if (playing == 2){
                                playing = 1;
                            }

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
        SDL_RenderPresent(ren);
    }
     

    
/*-------------------------------------------------------------------------*/
            //Close all
/*-------------------------------------------------------------------------*/
    free(py);
    free(px);
    free(wy);
    free(wh);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}




void SDL_ExitWithError(const char *errorMsg) {
    SDL_Log("Error ; %s > %s\n", errorMsg, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_ExitWithError(errorMsg);
}


void StartingGame(SDL_Renderer *ren, int *playerX, int *playerY){

    erasePlayer(ren,*playerX,*playerY);

    *playerX = (WIDTH - *playerX) / 2;
    drawPlayer(ren,*playerX,*playerY);

}

void limit_FPS(unsigned int limit){
    unsigned int ticks = SDL_GetTicks();

    if(limit < ticks)
        return;
    else if(limit > ticks + 16){
        SDL_Delay(16);
    }
    else{
        SDL_Delay(limit-ticks);
    }
}


void drawWalls(SDL_Renderer *ren, int wallY, int wallH){
    if(SDL_SetRenderDrawColor(ren, 200,200,200, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }
    SDL_Rect leftWall;
    SDL_Rect rightWall;

    leftWall.x = 0;
    rightWall.x = WIDTH-30;

    leftWall.y = wallY;
    rightWall.y = wallY;

    leftWall.w = 30;
    rightWall.w = 30;

    leftWall.h = wallH;
    rightWall.h = wallH;

    if(SDL_RenderFillRect(ren, &leftWall) != 0){ 
                SDL_ExitWithError("can't draw left wall");
            }
    if(SDL_RenderFillRect(ren, &rightWall) != 0){ 
                SDL_ExitWithError("can't draw right wall");
            }
}

void eraseWalls(SDL_Renderer *ren, int wallY, int wallH){
    if(SDL_SetRenderDrawColor(ren, 0,0,0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }
    SDL_Rect leftWall;
    SDL_Rect rightWall;

    leftWall.x = 0;
    rightWall.x = WIDTH-30;

    leftWall.y = wallY;
    rightWall.y = wallY;

    leftWall.w = 30;
    rightWall.w = 30;

    leftWall.h = wallH;
    rightWall.h = wallH;

    if(SDL_RenderFillRect(ren, &leftWall) != 0){ 
                SDL_ExitWithError("can't draw left wall");
            }
    if(SDL_RenderFillRect(ren, &rightWall) != 0){ 
                SDL_ExitWithError("can't draw right wall");
            }
}

void drawPlayer(SDL_Renderer *ren, int playerX, int playerY){
    if(SDL_SetRenderDrawColor(ren, 0,200,0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }

    SDL_Rect player;
    player.y = playerY;
    player.x = playerX;
    player.w = PlayerWidth;
    player.h = PlayerHeight;
    
    if(SDL_RenderFillRect(ren, &player) != 0){ 
                SDL_ExitWithError("can't draw player");
    }
}

void erasePlayer(SDL_Renderer *ren, int playerX, int playerY){
    if(SDL_SetRenderDrawColor(ren, 0,0,0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }

    SDL_Rect player;
    player.y = playerY;
    player.x = playerX;
    player.w = PlayerWidth;
    player.h = PlayerHeight;
    
    if(SDL_RenderFillRect(ren, &player) != 0){ 
                SDL_ExitWithError("can't draw player");
    }
}

void moveRight(SDL_Renderer *ren,int *playerX, int *playerY){
    erasePlayer(ren,*playerX,*playerY);

    *playerX += 15;
    drawPlayer(ren,*playerX,*playerY);
}

void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY){
    erasePlayer(ren,*playerX,*playerY);

    *playerX -= 15;
    drawPlayer(ren,*playerX,*playerY);
}