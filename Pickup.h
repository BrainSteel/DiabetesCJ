//
//  Pickup.h
//  DiaCat
//
//  Created by Jesse Pritchard on 2/6/16.
//  Copyright © 2016 Jesse Pritchard. All rights reserved.
//

#ifndef Pickup_h
#define Pickup_h

typedef struct sPickup{
    int ID;
    int active;
    int x, y;

    int frequency;
    
    char name[200];
    char file[200];
    
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

int PCK_InitializeFromFile(const char* config);
int PCK_InitializeCopy(Pickup* pickups, int num);
int PCK_AddPickup(Pickup add);
Pickup PCK_GetWeightedPickup();
Pickup PCK_GetUnweightedPickup();
Pickup* PCK_GetAllPickups(int* num);
void PCK_FreePickups();

#endif /* Pickup_h */
