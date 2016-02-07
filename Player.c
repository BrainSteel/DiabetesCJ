
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
    
    return player;
}