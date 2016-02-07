
#ifndef Player_h
#define Player_h

#include "SDL.h"
#include "Level.h"
#include "Direction.h"



typedef enum eGender {
    GEN_MALE,
    GEN_FEMALE
} Gender;

typedef enum eBodyType {
    BT_LEAN,
    BT_STANDARD,
    BT_OVERWEIGHT,
    BT_OBESE
} BodyType;

typedef struct sMovingEntity {
    Direction facing;
    
    SDL_Texture* tex[4];
    
    float speed;
    
    float x, y;
    int width, height;
    
    int tilex, tiley;
    
} MovingEntity;


#define TIME_FACTOR 150
#define SECONDS_TO_MINUTES ((TIME_FACTOR / 60.0))

#define PLAYER_SPEED 0.1
#define DEATH_SPEED 0.2
#define SPRINT_SPEED 0.4


typedef struct sPlayer {
    MovingEntity entity;
    float defaultx;
    float defaulty;
    float deftilex;
    float deftiley;
    
    /* Given by the user at runtime */
    Gender gender;
    float weight;
    int height;
    char name[200];
    
    /* BodyType-dependent variables */
    BodyType type;
    int ICF;
    int ISF;
    float BMI;
    
    /* List of attributes */
    float glucose;
    float insulin;
    float sensitivity;
    
    float CHO;
    
    float hydration;
    
    float stress;
    float illness;
    
    float alcohol;
    
    int blocksran;
    int blocksconsecutive;
    
    float sleep;
    float overwork;
    
    float vitality;
    
    time_t frames;
    time_t secondsreal;
    time_t minutessim;
    int lasthour;
    int day;
    
    int sprinting;
    
    int (*movequeue)[2];
    int deathdistance;
    MovingEntity death;
    
} Player;

Player* PLR_Initialize(SDL_Renderer* rend, Level* lvl, Gender gender, int weight, int height, const char* name);
void PLR_DestroyPlayer(Player* player);
void PLR_StartNewDay(Player* player);
void PLR_UpdateAll(Player* player, Level* lvl, time_t frames);
void PLR_UpdateHealth(Player* player, time_t frames);
void PLR_ConsumePickup(Player* player, Pickup* pickup);
void PLR_MoveEntity(MovingEntity* entity, Level* lvl, time_t frames, int entityplayer);


#endif /* Player_h */
