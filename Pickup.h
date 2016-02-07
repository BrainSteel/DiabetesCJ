

#ifndef Pickup_h
#define Pickup_h

#include "SDL.h"

typedef struct sPickup{
    int ID;
    int active;
    int x, y;

    int frequency;
    
    char name[200];
    char file[200];
    SDL_Texture* tex;
    
    /* Food modifiers */
    float CHOmod;
    float caloriesmod;
    // float stressmod;
    
    /* Hydration modifier */
    float watermod;
    
    /* Alcohol modifier */
    float alcoholmod;
    
    /* Insulin modifier */
    float insulinmod;
    
} Pickup;

int PCK_InitializeFromFile(const char* config, SDL_Renderer* rend);
int PCK_InitializeCopy(Pickup* pickups, int num);
int PCK_AddPickup(Pickup add);
Pickup PCK_GetWeightedPickup(int x, int y, int active);
Pickup PCK_GetUnweightedPickup(int x, int y, int active);
Pickup* PCK_GetAllPickups(int* num);
void PCK_FreePickups();

#endif /* Pickup_h */
