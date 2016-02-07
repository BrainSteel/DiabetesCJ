//#define NOLOGCHANGE
//#define NOINITCHANGE
#include "SDL.h"
#include "xorshift.h"
#include "stdlib.h"
#include "Level.h"

#define SCREENW 1280
#define SCREENH 720

int main(int argc, char** argv){
    //Start SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL Failed Initialization!");
        return 1;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* rend = NULL;
    SDL_CreateWindowAndRenderer(SCREENW, SCREENH, SDL_WINDOW_SHOWN, &window, &rend);

    SDL_Surface* GRASS = SDL_LoadBMP("GRASS.bmp");
    if(!GRASS)
    {
        printf("ERROR-> GRASS NOT LOADED");
        return 1;
    }
    SDL_Surface* Death_F = SDL_LoadBMP("Death_F.bmp");
    if(!Death_F)
    {
        printf("ERROR-> Death_F NOT LOADED");
        return 1;
    }
    SDL_Surface* Death_B = SDL_LoadBMP("Death_B.bmp");
    if(!Death_B)
    {
        printf("ERROR-> Death_B NOT LOADED");
        return 1;
    }
    SDL_Surface* Death_R = SDL_LoadBMP("Death_R.bmp");
    if(!Death_R)
    {
        printf("ERROR-> Death_R NOT LOADED");
        return 1;
    }
    SDL_Surface* Man_F = SDL_LoadBMP("Man_F.bmp");
    if(!Man_F)
    {
        printf("ERROR-> Man_F NOT LOADED");
        return 1;
    }
    SDL_Surface* Man_B = SDL_LoadBMP("Man_B.bmp");
    if(!Man_B)
    {
        printf("ERROR-> Man_B NOT LOADED");
        return 1;
    }
    SDL_Surface* Man_R = SDL_LoadBMP("Man_R.bmp");
    if(!Man_R)
    {
        printf("ERROR-> Man_R NOT LOADED");
        return 1;
    }
    SDL_Surface* Woman_F = SDL_LoadBMP("Woman_F.bmp");
    if(!Woman_F)
    {
        printf("ERROR-> Woman_F NOT LOADED");
        return 1;
    }
    SDL_Surface* Woman_B = SDL_LoadBMP("Woman_B.bmp");
    if(!Woman_B)
    {
        printf("ERROR-> Woman_B NOT LOADED");
        return 1;
    }
    SDL_Surface* Woman_R = SDL_LoadBMP("Woman_R.bmp");
    if(!Woman_R)
    {
        printf("ERROR-> Woman_R NOT LOADED");
        return 1;
    }

    SDL_Texture* GrassTex = SDL_CreateTextureFromSurface(rend, GRASS);
    SDL_Texture* Death_Ftex = SDL_CreateTextureFromSurface(rend, Death_F);
    SDL_Texture* Death_Btex = SDL_CreateTextureFromSurface(rend, Death_B);
    SDL_Texture* Death_Rtex = SDL_CreateTextureFromSurface(rend, Death_R);
    SDL_Texture* Man_Ftex = SDL_CreateTextureFromSurface(rend, Man_F);
    SDL_Texture* Man_Btex = SDL_CreateTextureFromSurface(rend, Man_B);
    SDL_Texture* Man_Rtex = SDL_CreateTextureFromSurface(rend, Man_R);
    SDL_Texture* Woman_Ftex = SDL_CreateTextureFromSurface(rend, Woman_F);
    SDL_Texture* Woman_Btex = SDL_CreateTextureFromSurface(rend, Woman_B);
    SDL_Texture* Woman_Rtex = SDL_CreateTextureFromSurface(rend, Woman_R);


    SDL_FreeSurface(GRASS);
    SDL_FreeSurface(Death_F);
    SDL_FreeSurface(Death_B);
    SDL_FreeSurface(Death_R);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    seedxorshift(time(0), SDL_GetTicks());


    Level* lvl = LVL_Generate(0);
    if (!lvl) {
        return 1;
    }

    Wall prev;
    int initialized = 0;

    Wall wall;

#define RECT_WIDTH 2

    double stepw = SCREENW / lvl->width;
    double steph = SCREENH / lvl->height;

    SDL_Event event;
    SDL_Rect green;
    green.w = 0;
    green.h = 0;
    green.x = 0;
    green.y = 0;
    do {

        SDL_WaitEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mousex = event.button.x;
            int mousey = event.button.y;

            int xpos = (int) (mousex / stepw);
            int ypos = (int) (mousey / steph);

            SDL_Rect rect;
            rect.x = xpos * stepw;
            rect.y = ypos * steph;
            rect.w = stepw;
            rect.h = steph;

            if (mousex < rect.x + RECT_WIDTH) {
                wall.x = xpos;
                wall.y = ypos;
                wall.dir = DIR_LEFT;
                if (LVL_AddWall(lvl, wall, initialized ? &prev : NULL)) {
                    prev = wall;
                    initialized = 1;
                }
            }
            else if (mousex > rect.x + stepw - RECT_WIDTH) {
                wall.x = xpos;
                wall.y = ypos;
                wall.dir = DIR_RIGHT;
                if (LVL_AddWall(lvl, wall, initialized ? &prev : NULL)) {
                    prev = wall;
                    initialized = 1;
                }
            }
            else if (mousey < rect.y + RECT_WIDTH) {
                wall.x = xpos;
                wall.y = ypos;
                wall.dir = DIR_UP;
                if (LVL_AddWall(lvl, wall, initialized ? &prev : NULL)) {
                    prev = wall;
                    initialized = 1;
                }
            }
            else if (mousey > rect.y + steph - RECT_WIDTH) {
                wall.x = xpos;
                wall.y = ypos;
                wall.dir = DIR_DOWN;
                if (LVL_AddWall(lvl, wall, initialized ? &prev : NULL)) {
                    prev = wall;
                    initialized = 1;
                }
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            int mousex = event.motion.x;
            int mousey = event.motion.y;

            int xpos = (int) (mousex / stepw);
            int ypos = (int) (mousey / steph);

            SDL_Rect rect;
            rect.x = xpos * stepw;
            rect.y = ypos * steph;
            rect.w = stepw;
            rect.h = steph;

            if (mousex < rect.x + RECT_WIDTH) {
                green.x = rect.x;
                green.y = rect.y;
                green.w = RECT_WIDTH;
                green.h = steph;
            }
            else if (mousex > rect.x + stepw - RECT_WIDTH) {
                green.x = rect.x + stepw - RECT_WIDTH;
                green.y = rect.y;
                green.w = RECT_WIDTH;
                green.h = steph;
            }
            else if (mousey < rect.y + RECT_WIDTH) {
                green.x = rect.x;
                green.y = rect.y;
                green.w = stepw;
                green.h = RECT_WIDTH;
            }
            else if (mousey > rect.y + steph - RECT_WIDTH) {
                green.x = rect.x;
                green.y = rect.y + steph - RECT_WIDTH;
                green.w = stepw;
                green.h = RECT_WIDTH;
            }
            else {
                green.x = 0;
                green.y = 0;
                green.w = 0;
                green.h = 0;
            }
        }

        SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
        //SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

        int i;
        int j;
        for(i = 0; i < lvl->width; i++)
        {
            for(j = 0; j < lvl->height; j++)
            {
                SDL_Rect sdlr;
                sdlr.x = i * stepw;
                sdlr.y = j * steph;
                sdlr.w = stepw;
                sdlr.h = steph;
                SDL_RenderCopy(rend, GrassTex, NULL, &sdlr);
            }
        }
        int count;
        for (count = 0; count < lvl->width; count++) {
            SDL_RenderDrawLine(rend, count * stepw, 0, count * stepw, SCREENH);
        }

        for (count = 0; count < lvl->height; count++) {
            SDL_RenderDrawLine(rend, 0, count * steph, SCREENW, count * steph);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
        int xpos, ypos;
        for (xpos = 0; xpos < lvl->width; xpos++) {
            for (ypos = 0; ypos < lvl->height; ypos++) {
                Cell cell = LVL_GetCell(lvl, xpos, ypos);
                if (DIR_ContainsDirection(cell.mask, DIR_LEFT)) {
                    SDL_Rect rect;
                    rect.x = xpos * stepw;
                    rect.y = ypos * steph;
                    rect.w = RECT_WIDTH;
                    rect.h = steph;
                    SDL_RenderFillRect(rend, &rect);
                }
                if (DIR_ContainsDirection(cell.mask, DIR_RIGHT)) {
                    SDL_Rect rect;
                    rect.x = (xpos + 1) * stepw - RECT_WIDTH;
                    rect.y = ypos * steph;
                    rect.w = RECT_WIDTH;
                    rect.h = steph;
                    SDL_RenderFillRect(rend, &rect);
                }
                if (DIR_ContainsDirection(cell.mask, DIR_UP)) {
                    SDL_Rect rect;
                    rect.x = xpos * stepw;
                    rect.y = ypos * steph;
                    rect.w = stepw;
                    rect.h = RECT_WIDTH;
                    SDL_RenderFillRect(rend, &rect);
                }
                if (DIR_ContainsDirection(cell.mask, DIR_DOWN)) {
                    SDL_Rect rect;
                    rect.x = xpos * stepw;
                    rect.y = (ypos + 1) * steph - RECT_WIDTH;
                    rect.w = stepw;
                    rect.h = RECT_WIDTH;
                    SDL_RenderFillRect(rend, &rect);
                }
            }
        }


        SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &green);

        SDL_RenderPresent(rend);

    } while (event.type != SDL_QUIT);


    free(lvl->grid);
    free(lvl);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rend);

    SDL_Quit();
    return 0;
}
