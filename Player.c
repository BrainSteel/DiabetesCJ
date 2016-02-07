
#include "stdlib.h"
#include "Player.h"
#include "string.h"
#include "Level.h"

Player* PLR_Initialize(Level* lvl, Gender gender, int weight, int height, const char* name) {
    
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
    
    player->glucose = 130;
    player->hydration = 72;
    player->illness = 0;
    player->stress = 0;
    player->sensitivity = 1.0;
    
    player->day = 0;
    
    return player;
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
    player->vitality = 100;
    player->blocksran = 0;
    player->blocksconsecutive = 0;
    player->alcohol = 0;
    player->sprinting = 0;
    
    player->frames = 0;
    player->secondsreal = 0;
    player->minutessim = 0;
    player->lasthour = 0;
    player->day++;
    
    /* List of attributes */
    // player->glucose = ?
    // player->insulin = ?
    // player->sensitivity = ?
    // player->CHO = ?
    // player->hydration = ?
    
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
    player->glucose = glucose;
    player->hydration = hydration;
    player->CHO = CHO;
    player->stress = stress;
    player->sensitivity = sensitivity;
}









