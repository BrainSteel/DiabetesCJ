
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
    
    int speed;
    
    double x, y;
    int width, height;
    
    int tilex, tiley;
    
} MovingEntity;


#define GLUCOSE_START 130


typedef struct sPlayer {
    MovingEntity entity;
    
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
    int dailycal;
    
    float hydration;
    int dailywater;
    
    float stress;
    float illness;
    
    float alcohol;
    
    float dailyexercise;
    int blocksran;
    int blocksconsecutive;
    
    float sleep;
    float overwork;
    
    float vitality;
    
    time_t frames;
    time_t secondsreal;
    time_t minutessim;
    
    int sprinting;
    
    int deathdistance;
    MovingEntity death;
    
} Player;

Player* PLR_Initialize(Level* lvl, Gender gender, int weight, int height, const char* name);
void PLR_UpdateHealth(Player* player, time_t frames);
void PLR_MoveEntity(MovingEntity* entity, time_t frames);


#endif /* Player_h */
