
#include "stdlib.h"
#include "string.h"
#include "Pickup.h"
#include "stdio.h"

static Pickup* AllPickups = NULL;
static int NumPickups = 0;


int PCK_InitializeFromFile(const char* config) {
    FILE* file = fopen(config, "r");
    if (!file) {
        return -1;
    }
    
    
    char namebuf[200];
    
    float CHO, calories, water, alcohol, insulin;
    int frequency;
    
    char filebuf[200];
    
    while (fscanf(file, "%*[^\"]\"%s\"%*[^:]: %f, %f, %f, %f, %f, %d, \"%s\"\n",
                  namebuf, &CHO, &calories, &water, &alcohol, &insulin, &frequency, filebuf) == 8) {
        Pickup* tmp = realloc(AllPickups, NumPickups + 1);
        if (tmp) {
            AllPickups = tmp;
            NumPickups++;
            
            AllPickups[NumPickups - 1].active = 0;
            AllPickups[NumPickups - 1].x = 0;
            AllPickups[NumPickups - 1].y = 0;
            AllPickups[NumPickups - 1].ID = 0;
            
            strcpy(AllPickups[NumPickups - 1].name, namebuf);
            strcpy(AllPickups[NumPickups - 1].file, filebuf);
            
            AllPickups[NumPickups - 1].CHOmod = CHO;
            AllPickups[NumPickups - 1].caloriesmod = calories;
            AllPickups[NumPickups - 1].watermod = water;
            AllPickups[NumPickups - 1].alcoholmod = alcohol;
            AllPickups[NumPickups - 1].insulinmod = insulin;
            AllPickups[NumPickups - 1].frequency = frequency;
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
    
    
}

Pickup PCK_GetUnweightedPickup() {
    
}

Pickup* PCK_GetAllPickups(int* num) {
    if (num) {
        *num = NumPickups;
    }
    return AllPickups;
}

void PCK_FreePickups() {
    free(AllPickups);
    NumPickups = 0;
}





