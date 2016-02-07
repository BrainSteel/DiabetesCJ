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
#define FRAMERATE 60

int main(int argc, char** argv){
    //Start SDL
    if (Extra_SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL Failed Initialization!");
        return 1;
    }
    
    AUD_Sound* music = AUD_LoadWAV("Level Up.wav", 1);
    AUD_Play(music);

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

    SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    seedxorshift(time(0), SDL_GetTicks());

    Level* lvl = LVL_Generate(0, MAPW, MAPH);
    if (!lvl) {
        return 1;
    }
    
    char name[200];
    name[0] = '\0';
    Gender gender;
    char cweight[200];
    char cheight[200];
    
    int currentchar = 0;
    SDL_Event input;
    int done = 0;
    do {
        SDL_WaitEvent(&input);
        
        if (input.type == SDL_KEYDOWN) {
            if (input.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            }
            else if (input.key.keysym.sym == SDLK_LSHIFT || input.key.keysym.sym == SDLK_RSHIFT) {
                name[currentchar] = 0;
            }
            else if (input.key.keysym.sym == SDLK_BACKSPACE) {
                currentchar--;
                name[currentchar] = 0;
            }
            else if (input.key.keysym.sym == SDLK_SPACE) {
                name[currentchar++] = ' ';
                name[currentchar] = 0;
            }
            else {
                name[currentchar++] = SDL_GetKeyName(input.key.keysym.sym)[0];
                name[currentchar] = '\0';
            }
        }
        else if (input.type == SDL_QUIT) {
            return 0;
        }
        
        
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
        FNT_DrawText(rend, font, "ENTER A NAME", 0, 0, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        FNT_DrawText(rend, font, name, 0, 60, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        SDL_RenderPresent(rend);
    } while (!done);
    
    
    SDL_Rect mrect, frect;
    mrect.x = SCREENW / 3;
    mrect.y = 2 * SCREENH / 3;
    mrect.w = 100;
    mrect.h = 50;
    
    frect.x = 2 * SCREENW / 3;
    frect.y = 2 * SCREENH / 3;
    frect.w = 100;
    frect.h = 50;
    done = 0;
    do {
        SDL_WaitEvent(&input);
        
        if (input.type == SDL_MOUSEBUTTONDOWN) {
            if (input.button.x > mrect.x && input.button.x < mrect.x + mrect.w &&
                input.button.y > mrect.y && input.button.y < mrect.y + mrect.h) {
                gender = GEN_MALE;
                done = 1;
            }
            else if (input.button.x > frect.x && input.button.x < frect.x + frect.w &&
                     input.button.y > frect.y && input.button.y < frect.y + frect.h) {
                gender = GEN_FEMALE;
                done = 1;
            }
        }
        else if (input.type == SDL_QUIT) {
            return 0;
        }
        
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
        FNT_DrawText(rend, font, "SELECT A GENDER", 0, 0, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        
        SDL_SetRenderDrawColor(rend, 0, 40, 70, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &mrect);
        SDL_RenderFillRect(rend, &frect);
        
        FNT_DrawText(rend, font, "M", mrect.x + 42, mrect.y + 10, 30, 0);
        FNT_DrawText(rend, font, "F", frect.x + 42, frect.y + 10, 30, 0);
        
        
        SDL_RenderPresent(rend);
    } while (!done);
    
    done = 0;
    currentchar = 0;
    do {
        SDL_WaitEvent(&input);
        
        if (input.type == SDL_KEYDOWN) {
            if (input.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            }
            else if (input.key.keysym.sym == SDLK_BACKSPACE) {
                currentchar--;
                cweight[currentchar] = 0;
            }
            else {
                char test = SDL_GetKeyName(input.key.keysym.sym)[0];
                if (test >= '0' && test <= '9') {
                    cweight[currentchar++] = test;
                    cweight[currentchar] = 0;
                }
            }
        }
        else if (input.type == SDL_QUIT) {
            return 0;
        }
        
        
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
        FNT_DrawText(rend, font, "ENTER A WEIGHT IN POUNDS", 0, 0, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        FNT_DrawText(rend, font, cweight, 0, 60, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        SDL_RenderPresent(rend);
    } while (!done);
    
    done = 0;
    currentchar = 0;
    do {
        SDL_WaitEvent(&input);
        
        if (input.type == SDL_KEYDOWN) {
            if (input.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            }
            else if (input.key.keysym.sym == SDLK_BACKSPACE) {
                currentchar--;
                cheight[currentchar] = 0;
            }
            else {
                char test = SDL_GetKeyName(input.key.keysym.sym)[0];
                if (test >= '0' && test <= '9') {
                    cheight[currentchar++] = test;
                    cheight[currentchar] = 0;
                }
            }
        }
        else if (input.type == SDL_QUIT) {
            return 0;
        }
        
        
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
        FNT_DrawText(rend, font, "ENTER A HEIGHT IN INCHES", 0, 0, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        FNT_DrawText(rend, font, cheight, 0, 60, 50, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
        SDL_RenderPresent(rend);
    } while (!done);

    Player* player = PLR_Initialize(rend, lvl, gender, atoi(cweight), atoi(cheight), name);

#define RECT_WIDTH 2

    double stepw = MAPW / lvl->width;
    double steph = MAPH / lvl->height;

    SDL_Event event;
    unsigned int time;
    int quit = 0;
    do {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if( (event.button.x > 950 && event.button.x < 1070) && (event.button.y > 650 && event.button.y < 690) ) {
                    LVL_DestroyLevel(lvl);
                    lvl = LVL_Generate(0, MAPW, MAPH);
                    PLR_DestroyPlayer(player);
                    player = PLR_Initialize(rend, lvl, gender, atoi(cweight), atoi(cheight), name);
                }

            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LSHIFT:
                        player->sprinting = !player->sprinting;
                        break;
                        
                    case SDLK_w:
                        if (player->entity.facing != DIR_DOWN) {
                            player->entity.facing = DIR_UP;
                        }
                        break;
                    
                    case SDLK_a:
                        if (player->entity.facing != DIR_RIGHT) {
                            player->entity.facing = DIR_LEFT;
                        }
                        break;
                        
                    case SDLK_s:
                        if (player->entity.facing != DIR_UP) {
                            player->entity.facing = DIR_DOWN;
                        }
                        break;
                        
                    case SDLK_d:
                        if (player->entity.facing != DIR_LEFT) {
                            player->entity.facing = DIR_RIGHT;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            else if(event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        PLR_UpdateAll(player, lvl, 1);
        if (player->lasthour == 24) {
            SDL_SetRenderDrawColor(rend, 0, 255, 0, 127);
            SDL_Event win;
            do {
                SDL_WaitEvent(&win);
                SDL_RenderClear(rend);
                FNT_DrawText(rend, font, "YOU SURVIVED THE DAY", 0, -32, 25, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
                FNT_DrawText(rend, font, "PRESS ENTER KEY TO CONTINUE", 0, 0, 25, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
                SDL_RenderPresent(rend);
            } while (!(win.type == SDL_KEYDOWN && win.key.keysym.sym == SDLK_RETURN));
            LVL_DestroyLevel(lvl);
            lvl = LVL_Generate(player->day + 1, MAPW, MAPH);
            PLR_StartNewDay(player);
        }
        if (player->vitality == 0) {
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 127);
            SDL_Event win;
            do {
                SDL_WaitEvent(&win);
                SDL_RenderClear(rend);
                FNT_DrawText(rend, font, "YOU SUCCUMBED TO DEATH", 0, -32, 25, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
                FNT_DrawText(rend, font, "PRESS ENTER KEY TO CONTINUE", 0, 0, 25, FNT_ALIGNCENTERX | FNT_ALIGNCENTERY);
                SDL_RenderPresent(rend);
            } while (!(win.type == SDL_KEYDOWN && win.key.keysym.sym == SDLK_RETURN));
            LVL_DestroyLevel(lvl);
            lvl = LVL_Generate(0, MAPW, MAPH);
            PLR_DestroyPlayer(player);
            player = PLR_Initialize(rend, lvl, gender, atoi(cweight), atoi(cheight), name);
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
        FNT_DrawText(rend, font, player->name, 800, 700, 30, 0);
        if (player->sprinting) {
            FNT_DrawText(rend, font, "PRESS SHIFT TO WALK", 800, 5, 20, 0);
        }
        else FNT_DrawText(rend, font, "PRESS SHIFT TO SPRINT", 800, 5, 20, 0);
        FNT_DrawText(rend, font, "ATTRIBUTES", 880, 50, 30, 0);



        SDL_Rect Attribute_HUB;
            Attribute_HUB.x = 799;
            Attribute_HUB.y = 100;
            Attribute_HUB.w = player->hydration * 4;
            Attribute_HUB.h = steph;

        SDL_Rect Attribute_HUB2;
            Attribute_HUB2.x = 799;
            Attribute_HUB2.y = 125;
            Attribute_HUB2.w = player->CHO * 2;
            Attribute_HUB2.h = steph;

        SDL_Rect Attribute_HUB3;
            Attribute_HUB3.x = 799;
            Attribute_HUB3.y = 150;
            Attribute_HUB3.w = player->glucose * 2;
            Attribute_HUB3.h = steph;

        SDL_Rect Attribute_HUB4;
            Attribute_HUB4.x = 799;
            Attribute_HUB4.y = 175;
            Attribute_HUB4.w = player->alcohol * 50;
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
        FNT_DrawText(rend, font, "CARBOHYDRATES", 900, 125, 20, 0);
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
        SDL_Rect vitrect;
        vitrect.x = 1000;
        vitrect.y = 200;
        vitrect.w = 125 - 12.5 * player->vitality;
        vitrect.h = steph;
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(rend, &vitrect);
        
        SDL_Rect Attribute_Sleep;
            Attribute_Sleep.x = 800;
            Attribute_Sleep.y = 225;
            Attribute_Sleep.w = stepw * 20;
            Attribute_Sleep.h = steph;

        SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
        FNT_DrawText(rend, font, "LAST NIGHT SLEEP" , 800, 225, 20, 0);
        char buf[200];
        sprintf(buf, "%d HOURS", (int)player->sleep);
        FNT_DrawText(rend, font, buf, 800, 250, 20, 0);
        
        sprintf(buf, "%d INCHES TALL", player->height);
        FNT_DrawText(rend, font, buf, 800, 275, 20, 0);
        sprintf(buf, "%d POUNDS IN WEIGHT", (int)player->weight);
        FNT_DrawText(rend, font, buf, 800, 300, 20, 0);
        sprintf(buf, "%d HOURS REMAINING ON", 24 - player->lasthour);
        FNT_DrawText(rend, font, buf, 800, 325, 20, 0);
        sprintf(buf, "DAY %d", player->day);
        FNT_DrawText(rend, font, buf, 800, 350, 20, 0);
        
        SDL_Rect prect;
        prect.x = player->entity.x - player->entity.width / 2;
        prect.y = player->entity.y - player->entity.height / 2;
        prect.w = player->entity.width;
        prect.h = player->entity.height;
        SDL_RenderCopy(rend, player->entity.tex[player->entity.facing], NULL, &prect);

        SDL_RenderPresent(rend);

        if(SDL_GetTicks() - time < (unsigned)1000/FRAMERATE) {
#ifdef FORCE_TIME
            timeBeginPeriod(1);
#endif
            SDL_Delay(((1000/FRAMERATE) - (SDL_GetTicks() - time)));
#ifdef FORCE_TIME
            timeEndPeriod(1);
#endif
        }
        
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

