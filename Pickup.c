
#include "stdlib.h"
#include "string.h"
#include "Pickup.h"
#include "stdio.h"
#include "xorshift.h"

static Pickup* AllPickups = NULL;
static int NumPickups = 0;


int PCK_InitializeFromFile(const char* config, SDL_Renderer* rend) {
    FILE* file = fopen(config, "r");
    if (!file) {
        return -1;
    }
    
    
    char namebuf[200];
    
    float CHO, calories, water, alcohol, insulin;
    int frequency;
    
    char filebuf[200];
    
    int index = 0;
    
    while (fscanf(file, "%*[^\"]\"%s\"%*[^:]: %f, %f, %f, %f, %f, %d, \"%s\"\n",
                  namebuf, &CHO, &calories, &water, &alcohol, &insulin, &frequency, filebuf) == 8) {
        Pickup* tmp = realloc(AllPickups, NumPickups + 1);
        if (tmp) {
            AllPickups = tmp;
            NumPickups++;
            
            AllPickups[NumPickups - 1].active = 0;
            AllPickups[NumPickups - 1].x = 0;
            AllPickups[NumPickups - 1].y = 0;
            AllPickups[NumPickups - 1].ID = index;
            
            strcpy(AllPickups[NumPickups - 1].name, namebuf);
            strcpy(AllPickups[NumPickups - 1].file, filebuf);
            
            if (rend) {
                SDL_Surface* tmpsurf = SDL_LoadBMP(filebuf);
                if (tmpsurf) {
                    AllPickups[NumPickups - 1].tex = SDL_CreateTextureFromSurface(rend, tmpsurf);
                    SDL_FreeSurface(tmpsurf);
                }
            }
            
            AllPickups[NumPickups - 1].CHOmod = CHO;
            AllPickups[NumPickups - 1].caloriesmod = calories;
            AllPickups[NumPickups - 1].watermod = water;
            AllPickups[NumPickups - 1].alcoholmod = alcohol;
            AllPickups[NumPickups - 1].insulinmod = insulin;
            AllPickups[NumPickups - 1].frequency = frequency;
            
            index++;
        }
        else return NumPickups;
    }
    
    return NumPickups;
}


Pickup PCK_GetWeightedPickup() {
    int freqtotal = 0;
    int count;
    for (count = 0; count < NumPickups; count++) {
        freqtotal += AllPickups[count].frequency;
    }
    
    uint64_t rnd = xorshiftplus_uniform(freqtotal);
    freqtotal = 0;
    int index;
    for (index = 0; freqtotal < rnd; index++) {
        freqtotal += AllPickups[index].frequency;
    }
    
    return AllPickups[index - 1];
}

Pickup PCK_GetUnweightedPickup() {
    return AllPickups[xorshiftplus_uniform(NumPickups)];
}

Pickup* PCK_GetAllPickups(int* num) {
    if (num) {
        *num = NumPickups;
    }
    return AllPickups;
}

void PCK_FreePickups() {
    int i;
    for (i = 0; i < NumPickups; i++) {
        if (AllPickups[i].tex) {
            SDL_DestroyTexture(AllPickups[i].tex);
        }
    }
    free(AllPickups);
    NumPickups = 0;
}





