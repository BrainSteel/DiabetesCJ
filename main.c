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
#include "Font.h"

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
    
    SDL_Surface* Water = SDL_LoadBMP("Water.bmp");
    if(!Water)
    {
        printf("ERROR-> Water NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Water, SDL_TRUE, SDL_MapRGB(Water->format, 255, 0, 255));
    SDL_Surface* Alcohol = SDL_LoadBMP("Beer.bmp");
    if(!Alcohol)
    {
        printf("ERROR-> Alcohol NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Alcohol, SDL_TRUE, SDL_MapRGB(Alcohol->format, 255, 0, 255));
    SDL_Surface* Insulin = SDL_LoadBMP("Insulin.bmp");
    if(!Insulin)
    {
        printf("ERROR-> Insulin NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Insulin, SDL_TRUE, SDL_MapRGB(Insulin->format, 255, 0, 255));
    SDL_Surface* Heart = SDL_LoadBMP("Heart.bmp");
    if(!Heart)
    {
        printf("ERROR-> Heart NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Heart, SDL_TRUE, SDL_MapRGB(Heart->format, 255, 0, 255));
    SDL_Surface* Glucose = SDL_LoadBMP("Glucose.bmp");
    if(!Glucose)
    {
        printf("ERROR-> Glucose NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Glucose, SDL_TRUE, SDL_MapRGB(Glucose->format, 255, 0, 255));
    SDL_Surface* Doughnut = SDL_LoadBMP("Doughnut.bmp");
    if(!Doughnut)
    {
        printf("ERROR-> Doughnut NOT LOADED");
        return 1;
    }
    SDL_SetColorKey(Doughnut, SDL_TRUE, SDL_MapRGB(Doughnut->format, 255, 0, 255));

    SDL_Texture* GrassTex = SDL_CreateTextureFromSurface(rend, GRASS);
    SDL_Texture* Watertex = SDL_CreateTextureFromSurface(rend, Water);
    SDL_Texture* Alcoholtex = SDL_CreateTextureFromSurface(rend, Alcohol);
    SDL_Texture* Insulintex = SDL_CreateTextureFromSurface(rend, Insulin);
    SDL_Texture* Hearttex = SDL_CreateTextureFromSurface(rend, Heart);
    SDL_Texture* Glucosetex = SDL_CreateTextureFromSurface(rend, Glucose);
    SDL_Texture* Doughnuttex = SDL_CreateTextureFromSurface(rend, Doughnut);


    SDL_FreeSurface(GRASS);
    SDL_FreeSurface(Water);
    SDL_FreeSurface(Alcohol);
    SDL_FreeSurface(Insulin);
    SDL_FreeSurface(Heart);
    SDL_FreeSurface(Glucose);
    SDL_FreeSurface(Doughnut);

    SDL_Color color;
        color.r = 0;
        color.g = 40;
        color.b = 70;

    FNT_Font* font = FNT_InitFont(rend, "540x20Font.bmp", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 3, 4, color);

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
            if( (event.button.x > 950 && event.button.x < 1070) && (event.button.y > 650 && event.button.y < 690) ) {
                LVL_DestroyLevel(lvl);
                lvl = LVL_Generate(0, MAPW, MAPH);
                PLR_StartNewDay(player);
            }

        }
        else if (event.type == SDL_KEYDOWN) {
            PLR_UpdateAll(player, lvl, 1);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 75, 75, 75, SDL_ALPHA_OPAQUE);
        SDL_Rect Close;
            Close.x = 950;
            Close.y = 650;
            Close.w = stepw * 6;
            Close.h = steph * 2;
        SDL_RenderFillRect(rend, &Close);

        FNT_DrawText(rend, font, "RESTART" , 950, 660, 20, 0);


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


        SDL_Rect HUB_Title;
            HUB_Title.x = 799;
            HUB_Title.y = 45;
            HUB_Title.w = stepw * 20;
            HUB_Title.h = steph * 2;

        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);

        SDL_RenderDrawLine(rend, 799, 0, 1199, 0);
        SDL_RenderFillRect(rend, &HUB_Title);
        FNT_DrawText(rend, font, "PLAYER NAME", 800, 700, 30, 0);
        FNT_DrawText(rend, font, "PRESS CNTRL TO SPRINT", 800, 5, 20, 0);
        FNT_DrawText(rend, font, "ATTRIBUTES", 880, 50, 30, 0);



        SDL_Rect Attribute_HUB;
            Attribute_HUB.x = 799;
            Attribute_HUB.y = 100;
            Attribute_HUB.w = stepw * 20;
            Attribute_HUB.h = steph;

        SDL_Rect Attribute_HUB2;
            Attribute_HUB2.x = 799;
            Attribute_HUB2.y = 125;
            Attribute_HUB2.w = stepw * 20;
            Attribute_HUB2.h = steph;

        SDL_Rect Attribute_HUB3;
            Attribute_HUB3.x = 799;
            Attribute_HUB3.y = 150;
            Attribute_HUB3.w = stepw * 20;
            Attribute_HUB3.h = steph;

        SDL_Rect Attribute_HUB4;
            Attribute_HUB4.x = 799;
            Attribute_HUB4.y = 175;
            Attribute_HUB4.w = stepw * 20;
            Attribute_HUB4.h = steph;

        SDL_Rect pickrect2;
            pickrect2.x = 875;
            pickrect2.y = 100;
            pickrect2.w = stepw;
            pickrect2.h = steph;

        SDL_SetRenderDrawColor(rend, 0, 150, 100, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &Attribute_HUB);
        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "HYDRATION", 900, 100, 20, 0);
        SDL_RenderCopy(rend, Watertex, NULL, &pickrect2);

        SDL_Rect pickrect3;
            pickrect3.x = 875;
            pickrect3.y = 125;
            pickrect3.w = stepw;
            pickrect3.h = steph;

        SDL_SetRenderDrawColor(rend, 0, 0, 150, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &Attribute_HUB2);
        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "CALORIES", 900, 125, 20, 0);
        SDL_RenderCopy(rend, Doughnuttex, NULL, &pickrect3);

        SDL_Rect pickrect4;
            pickrect4.x = 875;
            pickrect4.y = 150;
            pickrect4.w = stepw;
            pickrect4.h = steph;

        SDL_SetRenderDrawColor(rend, 0, 150, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &Attribute_HUB3);
        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "GLUCOSE", 900, 150, 20, 0);
        SDL_RenderCopy(rend, Glucosetex, NULL, &pickrect4);

        SDL_Rect pickrect5;
            pickrect5.x = 875;
            pickrect5.y = 175;
            pickrect5.w = stepw;
            pickrect5.h = steph;

        SDL_SetRenderDrawColor(rend, 150, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &Attribute_HUB4);
        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "ALCOHOL", 900, 175, 20, 0);
        SDL_RenderCopy(rend, Alcoholtex, NULL, &pickrect5);


        FNT_DrawText(rend, font, "LIFE", 900, 200, 20, 0);
        SDL_Rect pickrect6;
        int xSpacer;
        int ySpacer = 200;
        for(xSpacer = 1000; xSpacer <= 1100; xSpacer = xSpacer + 25)
        {
                pickrect6.x = xSpacer;
                pickrect6.y = ySpacer;
                pickrect6.w = stepw;
                pickrect6.h = steph;
                SDL_RenderCopy(rend, Hearttex, NULL, &pickrect6);
        }

        SDL_Rect Attribute_Sleep;
            Attribute_Sleep.x = 800;
            Attribute_Sleep.y = 225;
            Attribute_Sleep.w = stepw * 20;
            Attribute_Sleep.h = steph;

        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "LAST NIGHT SLEEP" , 800, 225, 20, 0);

        SDL_RenderFillRect(rend, &green);
        
        SDL_Rect prect;
        prect.x = player->entity.x - player->entity.width / 2;
        prect.y = player->entity.y - player->entity.height / 2;
        prect.w = player->entity.width;
        prect.h = player->entity.height;
        SDL_RenderCopy(rend, player->entity.tex[player->entity.facing], NULL, &prect);

        SDL_RenderPresent(rend);

    } while (event.type != SDL_QUIT);
    
    PLR_DestroyPlayer(player);

    SDL_DestroyTexture(GrassTex);
    SDL_DestroyTexture(Watertex);
    SDL_DestroyTexture(Alcoholtex);
    SDL_DestroyTexture(Hearttex);
    SDL_DestroyTexture(Insulintex);
    SDL_DestroyTexture(Glucosetex);
    SDL_DestroyTexture(Doughnuttex);

    LVL_DestroyLevel(lvl);
    FNT_DestroyFont(font);


    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rend);


    SDL_Quit();
    return 0;




}





