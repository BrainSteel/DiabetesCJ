#include "math.h"
#include "stdlib.h"
#include "Player.h"
#include "string.h"
#include "Level.h"

#define ACCEPTABLE_ERROR 4

Player* PLR_Initialize(SDL_Renderer* rend, Level* lvl, Gender gender, int weight, int height, const char* name) {
    
    Player* player = malloc(sizeof(Player));
    if (!player) {
        return NULL;
    }
    player->gender = gender;
    player->weight = weight;
    player->height = height;
    strcpy(player->name, name);
    
    player->BMI = player->weight * 730 / (player->height * player->height);
    
    if (player->BMI <= 18.5) {
        player->type = BT_LEAN;
        player->ICF = 20;
        player->ISF = 50;
    }
    else if (player->BMI <= 25) {
        player->type = BT_STANDARD;
        player->ICF = 15;
        player->ISF = 30;
    }
    else if (player->BMI <= 30) {
        player->type = BT_OVERWEIGHT;
        player->ICF = 10;
        player->ISF = 20;
    }
    else {
        player->type = BT_OBESE;
        player->ICF = 5;
        player->ISF = 5;
    }

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(rend, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    
    Uint32 pixformat;
    if (gender == GEN_MALE) {
        SDL_Surface* ManRsurf = SDL_LoadBMP("Man_R.bmp");
        SDL_SetColorKey(ManRsurf, SDL_TRUE, SDL_MapRGB(ManRsurf->format, 255, 0, 255));
        player->entity.tex[DIR_RIGHT] = SDL_CreateTextureFromSurface(rend, ManRsurf);
        SDL_QueryTexture(player->entity.tex[DIR_RIGHT], &pixformat, NULL, NULL, NULL);
        player->entity.tex[DIR_LEFT] = SDL_CreateTexture(rend, pixformat, SDL_TEXTUREACCESS_TARGET, ManRsurf->w, ManRsurf->h);
        SDL_SetTextureBlendMode(player->entity.tex[DIR_LEFT], SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(rend, player->entity.tex[DIR_LEFT]);
        SDL_RenderClear(rend);
        SDL_RenderCopyEx(rend, player->entity.tex[DIR_RIGHT], NULL, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);
        SDL_SetRenderTarget(rend, NULL);
        SDL_FreeSurface(ManRsurf);
        
        SDL_Surface* ManFsurf = SDL_LoadBMP("Man_F.bmp");
        SDL_SetColorKey(ManFsurf, SDL_TRUE, SDL_MapRGB(ManFsurf->format, 255, 0, 255));
        player->entity.tex[DIR_DOWN] = SDL_CreateTextureFromSurface(rend, ManFsurf);
        SDL_FreeSurface(ManFsurf);
        
        SDL_Surface* ManBsurf = SDL_LoadBMP("Man_B.bmp");
        SDL_SetColorKey(ManBsurf, SDL_TRUE, SDL_MapRGB(ManBsurf->format, 255, 0, 255));
        player->entity.tex[DIR_UP] = SDL_CreateTextureFromSurface(rend, ManBsurf);
        SDL_FreeSurface(ManBsurf);
    }
    else {
        SDL_Surface* WomanRsurf = SDL_LoadBMP("Woman_R.bmp");
        SDL_SetColorKey(WomanRsurf, SDL_TRUE, SDL_MapRGB(WomanRsurf->format, 255, 0, 255));
        player->entity.tex[DIR_RIGHT] = SDL_CreateTextureFromSurface(rend, WomanRsurf);
        SDL_QueryTexture(player->entity.tex[DIR_RIGHT], &pixformat, NULL, NULL, NULL);
        player->entity.tex[DIR_LEFT] = SDL_CreateTexture(rend, pixformat, SDL_TEXTUREACCESS_TARGET, WomanRsurf->w, WomanRsurf->h);
        SDL_SetTextureBlendMode(player->entity.tex[DIR_LEFT], SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(rend, player->entity.tex[DIR_LEFT]);
        SDL_RenderClear(rend);
        SDL_RenderCopyEx(rend, player->entity.tex[DIR_RIGHT], NULL, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);
        SDL_SetRenderTarget(rend, NULL);
        SDL_FreeSurface(WomanRsurf);
        
        SDL_Surface* WomanFsurf = SDL_LoadBMP("Woman_F.bmp");
        SDL_SetColorKey(WomanFsurf, SDL_TRUE, SDL_MapRGB(WomanFsurf->format, 255, 0, 255));
        player->entity.tex[DIR_DOWN] = SDL_CreateTextureFromSurface(rend, WomanFsurf);
        SDL_FreeSurface(WomanFsurf);
        
        SDL_Surface* WomanBsurf = SDL_LoadBMP("Woman_B.bmp");
        SDL_SetColorKey(WomanBsurf, SDL_TRUE, SDL_MapRGB(WomanBsurf->format, 255, 0, 255));
        player->entity.tex[DIR_UP] = SDL_CreateTextureFromSurface(rend, WomanBsurf);
        SDL_FreeSurface(WomanBsurf);
    }
    
    SDL_Surface* DeathRsurf = SDL_LoadBMP("Death_R.bmp");
    SDL_SetColorKey(DeathRsurf, SDL_TRUE, SDL_MapRGB(DeathRsurf->format, 255, 0, 255));
    player->death.tex[DIR_RIGHT] = SDL_CreateTextureFromSurface(rend, DeathRsurf);
    SDL_QueryTexture(player->entity.tex[DIR_RIGHT], &pixformat, NULL, NULL, NULL);
    player->death.tex[DIR_LEFT] = SDL_CreateTexture(rend, pixformat, SDL_TEXTUREACCESS_TARGET, DeathRsurf->w, DeathRsurf->h);
    SDL_SetTextureBlendMode(player->death.tex[DIR_LEFT], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(rend, player->death.tex[DIR_LEFT]);
    SDL_RenderClear(rend);
    SDL_RenderCopyEx(rend, player->death.tex[DIR_RIGHT], NULL, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);

    SDL_SetRenderTarget(rend, NULL);
    SDL_FreeSurface(DeathRsurf);
    
    SDL_Surface* DeathFsurf = SDL_LoadBMP("Death_F.bmp");
    SDL_SetColorKey(DeathFsurf, SDL_TRUE, SDL_MapRGB(DeathFsurf->format, 255, 0, 255));
    player->death.tex[DIR_DOWN] = SDL_CreateTextureFromSurface(rend, DeathFsurf);
    SDL_FreeSurface(DeathFsurf);
    
    SDL_Surface* DeathBsurf = SDL_LoadBMP("Death_B.bmp");
    SDL_SetColorKey(DeathBsurf, SDL_TRUE, SDL_MapRGB(DeathBsurf->format, 255, 0, 255));
    player->death.tex[DIR_UP] = SDL_CreateTextureFromSurface(rend, DeathBsurf);
    SDL_FreeSurface(DeathBsurf);
    
    player->entity.width = lvl->cellwidth; //- 2 * ACCEPTABLE_ERROR;
    player->entity.height = lvl->cellheight; // - 2 * ACCEPTABLE_ERROR;
    
    player->entity.facing = DIR_DOWN;
    player->defaultx = lvl->totwidth / 2;
    player->defaulty = player->entity.height / 2;
    player->deftilex = lvl->width / 2;
    player->deftiley = 0;
    player->entity.speed = PLAYER_SPEED;

    player->glucose = 130;
    player->hydration = 72;
    player->illness = 0;
    player->stress = 0;
    player->sensitivity = 1.0;
    player->insulin = 1;
    
    player->day = -1;
    
    PLR_StartNewDay(player);
    
    return player;
}

void PLR_DestroyPlayer(Player* player) {
    if (player) {
        int i;
        for (i = 0; i < 4; i++) {
            if (player->entity.tex[i]) {
                SDL_DestroyTexture(player->entity.tex[i]);
            }
            if (player->death.tex[i]) {
                SDL_DestroyTexture(player->death.tex[i]);
            }
        }
    }
}

void PLR_StartNewDay(Player* player) {
    
    if (player->hydration > 191 && player->CHO > 120) {
        player->illness = 0;
        
    }
    else if (player->hydration > 72 && player->CHO > 120) {
        player->illness /= 2;
    }
    
    player->stress -= .5 * player->alcohol;
    if (player->glucose < 100 || player->glucose > 140) {
        player->illness++;
    }
    
    switch (player->gender) {
        case GEN_MALE:
            if (player->alcohol > 24) {
                player->illness++;
            }
            break;
            
        case GEN_FEMALE:
            if (player->alcohol > 12) {
                player->illness++;
            }
            break;
    }
    
    if (player->blocksran > 19) {
        player->illness--;
        player->stress--;
    }
    
    player->sleep = 8;
    if (player->glucose < 40 || player->glucose > 240) {
        player->sleep -= 5;
    }
    else if (player->glucose < 45 || player->glucose > 220) {
        player->sleep -= 4;
    }
    else if (player->glucose < 50 || player->glucose > 210) {
        player->sleep -= 3;
    }
    else if (player->glucose < 60 || player->glucose > 190) {
        player->sleep -= 2;
    }
    else if (player->glucose < 80 || player->glucose > 160) {
        player->sleep -= 1;
    }
 
    player->overwork = player->sleep < 6 ? 6 - player->sleep : 0;
    player->stress += player->overwork;
    
    
    /* Set up daily constants */
    player->vitality = 5;
    player->blocksran = 0;
    player->blocksconsecutive = 0;
    player->alcohol = 0;
    player->sprinting = 0;
    
    player->frames = 0;
    player->secondsreal = 0;
    player->minutessim = 0;
    player->lasthour = 0;
    player->day++;
    
    player->entity.facing = DIR_DOWN;
    player->entity.x = player->defaultx;
    player->entity.y = player->defaulty;
    player->entity.tilex = player->deftilex;
    player->entity.tiley = player->deftiley;
    player->entity.speed = PLAYER_SPEED;
    
    /* List of attributes */
    // player->glucose = ?
    // player->insulin = ?
    // player->sensitivity = ?
    // player->CHO = ?
    // player->hydration = ?
    
}

void PLR_UpdateAll(Player* player, Level* lvl, time_t frames) {
    if (player->sprinting) {
        player->entity.speed = SPRINT_SPEED;
    }
    else player->entity.speed = PLAYER_SPEED;
    PLR_UpdateHealth(player, frames);
    PLR_MoveEntity((MovingEntity*)player, lvl, frames, 1);
}

void PLR_UpdateHealth(Player* player, time_t frames) {
    player->frames += frames;
    player->secondsreal = player->frames / 60;
    player->minutessim = player->secondsreal * SECONDS_TO_MINUTES;
    
    float minutes = frames * (SECONDS_TO_MINUTES / 60.0);
    
    /* Set up the next set of player variables */
    float glucose = 130 - ( player->insulin * player->ISF ) * player->sensitivity;
    switch (player->type) {
        case BT_LEAN:
            glucose += 0.4 * player->CHO;
            break;
            
        case BT_STANDARD:
            glucose += 0.5 * player->CHO;
            break;
            
        case BT_OVERWEIGHT:
            glucose += 0.75 * player->CHO;
            break;
            
        case BT_OBESE:
            glucose += player->CHO;
            break;
    }
    
    float hydration = player->hydration - 0.5 * minutes;
    
    float CHO = player->CHO;
    if (player->minutessim / 60 > player->lasthour + 1) {
        player->lasthour++;
        CHO += player->stress;
        switch (player->type) {
            case BT_LEAN:
                glucose += .4 * CHO;
                break;
                
            case BT_STANDARD:
                glucose += .5 * CHO;
                break;
                
            case BT_OVERWEIGHT:
                glucose += .75 * CHO;
                break;
                
            case BT_OBESE:
                glucose += CHO;
                break;
        }
        if (glucose < 140 && glucose > 100 && hydration > 20) {
            player->vitality++;
            if (player->vitality > 10) {
                player->vitality = 10;
            }
        }
        else {
            player->vitality--;
        }
    }
    
    float stress = player->stress + player->illness;
    
    float blocksconsecutive = player->blocksconsecutive;
    float blocksrun = 0;
    float sensitivity = player->sensitivity;
    if (player->sprinting) {
        blocksrun = SPRINT_SPEED * frames;
        blocksconsecutive += blocksrun;
        hydration -= 0.05 * blocksrun;
        sensitivity += 0.1 * blocksrun;
    }
    else {
        if (blocksconsecutive > 3) {
            if (blocksconsecutive < 6) {
                CHO -= 15;
            }
            else {
                CHO -= 50 * blocksconsecutive / 6;
            }
            
            switch (player->type) {
                case BT_LEAN:
                    glucose += .4 * CHO;
                    break;
                    
                case BT_STANDARD:
                    glucose += .5 * CHO;
                    break;
                    
                case BT_OVERWEIGHT:
                    glucose += .75 * CHO;
                    break;
                    
                case BT_OBESE:
                    glucose += CHO;
                    break;
            }
        }
        blocksconsecutive = 0;
    }

    
    /* Update the player with new data */
    player->glucose = glucose > 0 ? glucose : 0;
    player->hydration = hydration > 0 ? hydration : 0;
    player->CHO = CHO > 0 ? CHO : 0;
    player->stress = stress;
    player->sensitivity = sensitivity;
}

void PLR_ConsumePickup(Player* player, Pickup* pickup) {
    if (!pickup->active) {
        return;
    }
    
    pickup->active = 0;
    player->CHO += pickup->CHOmod;
    player->hydration += pickup->watermod;
    player->alcohol += pickup->alcoholmod;
    player->insulin += pickup->insulinmod;
}

void PLR_MoveEntity(MovingEntity* entity, Level* lvl, time_t frames, int entityplayer) {
    entity->tilex = entity->x / lvl->cellwidth;
    entity->tiley = entity->y / lvl->cellheight;
    Cell cur = LVL_GetCell(lvl, entity->tilex, entity->tiley);
    int px = cur.x * lvl->cellwidth + lvl->cellwidth / 2;
    int py = cur.y * lvl->cellheight + lvl->cellheight / 2;
    int ishorizontal = entity->facing == DIR_LEFT || entity->facing == DIR_RIGHT;
    
    float movementleft;
    if (ishorizontal) {
        movementleft = frames * entity->speed * lvl->cellwidth;
    }
    else movementleft = frames * entity->speed * lvl->cellheight;
    
    while (movementleft > 0) {
        float move = 0;
        int breakloop = 1;
        if (ishorizontal) {
            move = abs(px - (int)entity->x);
            if (abs(py - (int)entity->y) < ACCEPTABLE_ERROR) {
                if (!DIR_ContainsDirection(cur.mask, entity->facing)) {
                    move += lvl->cellwidth;
                    breakloop = 0;
                }
            }
            else {
                Cell adj = py < entity->y ? LVL_GetCell(lvl, entity->tilex, entity->tiley - 1) : LVL_GetCell(lvl, entity->tilex, entity->tiley + 1);
                if (!DIR_ContainsDirection(cur.mask, entity->facing) && !DIR_ContainsDirection(adj.mask, entity->facing &&
                    (py < entity->y ? !DIR_ContainsDirection(cur.mask, DIR_UP) : !DIR_ContainsDirection(cur.mask, DIR_DOWN)))) {
                    move += lvl->cellwidth;
                    breakloop = 0;
                }
            }
        }
        else {
            move = abs(py - (int)entity->y);
            if (abs(px - (int)entity->x) < ACCEPTABLE_ERROR) {
                if (!DIR_ContainsDirection(cur.mask, entity->facing)) {
                    move += lvl->cellheight;
                    breakloop = 0;
                }
            }
            else {
                Cell adj = px < entity->x ? LVL_GetCell(lvl, entity->tilex + 1, entity->tiley) : LVL_GetCell(lvl, entity->tilex - 1, entity->tiley);
                if (!DIR_ContainsDirection(cur.mask, entity->facing) && !DIR_ContainsDirection(adj.mask, entity->facing) &&
                   (px < entity->x ? !DIR_ContainsDirection(cur.mask, DIR_RIGHT) : !DIR_ContainsDirection(cur.mask, DIR_LEFT))) {
                    move += lvl->cellheight;
                    breakloop = 0;
                }
            }
        }
        move = move > movementleft ? movementleft : move;
        movementleft -= move;
        int ix = entity->x, iy = entity->y;
        DIR_MoveCoord(&ix, &iy, entity->facing, move);
        entity->x = ix;
        entity->y = iy;
        entity->tilex = entity->x / lvl->cellwidth;
        entity->tiley = entity->y / lvl->cellheight;
        
        cur = LVL_GetCell(lvl, entity->tilex, entity->tiley);
        if (cur.item && entityplayer) {
            PLR_ConsumePickup((Player*)entity, cur.item);
        }
        
        px = cur.x * lvl->cellwidth + lvl->cellwidth / 2;
        py = cur.y * lvl->cellheight + lvl->cellheight / 2;
        
        if (breakloop) {
            break;
        }
    }
}







