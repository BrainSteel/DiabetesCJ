//#define NOLOGCHANGE
//#define NOINITCHANGE
#include "ExtraLayerSDL.h"
#include "xorshift.h"
#include "stdlib.h"
#include "Draw.h"
#include "Level.h"

#define SCREENW 1280
#define SCREENH 720

int main(int argc, char** argv){
    //Start SDL
    if(Extra_SDL_Init(SDL_INIT_EVERYTHING) < 0){
        GeneralLog("SDL Failed Initialization!");
        return 1;
    }
    
    SDL_Window* window = NULL;
    SDL_Renderer* rend = NULL;
    SDL_CreateWindowAndRenderer(SCREENW, SCREENH, SDL_WINDOW_SHOWN, &window, &rend);
    
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
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        
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
