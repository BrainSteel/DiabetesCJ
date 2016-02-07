

#include "Player.h"
#include "string.h"


Player* PLR_Initialize(Gender gender, int weight, int height, char* name) {
    
    Player* player = malloc(sizeof(Player));
    player->gender = gender;
    player->weight = weight;
    player->height = height;
    strcpy(name, player->name);
    
    player->BMI = player->weight * 730 / (player->height * player->height);
    
    if (player->BMI <= 18.5) {
        player->type = BT_LEAN;
    }
    else if (player->BMI > 18.5 && player->BMI < 27.5) {
        player->type = BT_STANDARD;
    }
    else if (player->BMI >= 27.5) {
        player->type = BT_OVERWEIGHT;
    }
}