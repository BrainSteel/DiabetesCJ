//#define NOLOGCHANGE
//#define NOINITCHANGE
#include "ExtraLayerSDL.h"
#include "Sound.h"
#include "time.h"
#include "xorshift.h"
#include "stdlib.h"
#include "Level.h"
#include "Pickup.h"
#include "Player.h"

#define MAPW 800
#define MAPH 800

#define SCREENW ((MAPW + 400))
#define SCREENH MAPH

int main(int argc, char** argv){
    //Start SDL
    if (Extra_SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL Failed Initialization!");
        return 1;
    }
    
    //AUD_Sound* music = AUD_LoadWAV("Level Up.wav", 1);
    //AUD_Play(music);

    SDL_Window* window = NULL;
    SDL_Renderer* rend = NULL;
    SDL_CreateWindowAndRenderer(SCREENW, SCREENH, SDL_WINDOW_SHOWN, &window, &rend);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    
    int numinit = PCK_InitializeFromFile("PickUps_Config.txt", rend);
    if (numinit <= 0) {
        printf("Failed to initialize Pickups!\n");
    }
    
    SDL_Surface* GRASS = SDL_LoadBMP("GRASS.bmp");
    if(!GRASS)
    {
        printf("ERROR-> GRASS NOT LOADED");
        return 1;
    }
    SDL_Texture* GrassTex = SDL_CreateTextureFromSurface(rend, GRASS);
    SDL_FreeSurface(GRASS);
    
    SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    seedxorshift(time(0), SDL_GetTicks());

    Level* lvl = LVL_Generate(0, MAPW, MAPH);
    if (!lvl) {
        return 1;
    }

    Player* player = PLR_Initialize(rend, lvl, GEN_MALE, 150, 74, "JESSE");
    
    
    Wall prev;
    int initialized = 0;

    Wall wall;

#define RECT_WIDTH 2

    double stepw = MAPW / lvl->width;
    double steph = MAPH / lvl->height;

    SDL_Event event;
    SDL_Rect green;
    green.w = 0;
    green.h = 0;
    green.x = 0;
    green.y = 0;
    do {

        SDL_WaitEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            player->entity.facing = DIR_GetClockwise(player->entity.facing);
            
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
        else if (event.type == SDL_KEYDOWN) {
            PLR_UpdateAll(player, lvl, 1);
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
        
        for (i = 0; i < lvl->numpickup; i++) {
            if (!lvl->pickups[i].active) {
                continue;
            }
            
            SDL_Rect pickrect;
            pickrect.x = lvl->pickups[i].x * stepw;
            pickrect.y = lvl->pickups[i].y * steph;
            pickrect.w = stepw;
            pickrect.h = steph;
            
            SDL_RenderCopy(rend, lvl->pickups[i].tex, NULL, &pickrect);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
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
        
        SDL_Rect prect;
        prect.x = player->entity.x - player->entity.width / 2;
        prect.y = player->entity.y - player->entity.height / 2;
        prect.w = player->entity.width;
        prect.h = player->entity.height;
        SDL_RenderCopy(rend, player->entity.tex[player->entity.facing], NULL, &prect);

        SDL_RenderPresent(rend);

    } while (event.type != SDL_QUIT);
    
    SDL_DestroyTexture(GrassTex);

    LVL_DestroyLevel(lvl);
    
    PLR_DestroyPlayer(player);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rend);

    SDL_Quit();
    return 0;
}
