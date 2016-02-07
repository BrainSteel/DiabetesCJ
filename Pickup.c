
#include "stdlib.h"
#include "string.h"
#include "Pickup.h"
#include "stdio.h"
#include "xorshift.h"

#define PCK_COLORKEY_R 255
#define PCK_COLORKEY_G 0
#define PCK_COLORKEY_B 255

static Pickup* AllPickups = NULL;
static int NumPickups = 0;


int PCK_InitializeFromFile(const char* config, SDL_Renderer* rend) {
    FILE* file = fopen(config, "r");
    if (!file) {
        printf("ERROR: Failed to load config file: %s\n", config);
        return -1;
    }


    char namebuf[200];

    float CHO, calories, water, alcohol, insulin;
    int frequency;

    char filebuf[200];

    int index = 0;
    int read;
    while ((read = fscanf(file, "%*[^\"]\"%[^\"]\"%*[^:]: %f, %f, %f, %f, %f, %d, \"%[^\"]\"",
                  namebuf, &CHO, &calories, &water, &alcohol, &insulin, &frequency, filebuf)) == 8) {
        Pickup* tmp = realloc(AllPickups, sizeof(Pickup) * (NumPickups + 1));
        if (tmp) {
            printf("\"%s\": %f, %f, %f, %f, %f, %d, \"%s\"\n", namebuf, CHO, calories, water, alcohol, insulin, frequency, filebuf);
            
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
                    SDL_SetColorKey(tmpsurf, SDL_TRUE, SDL_MapRGB(tmpsurf->format, PCK_COLORKEY_R, PCK_COLORKEY_G, PCK_COLORKEY_B));
                    AllPickups[NumPickups - 1].tex = SDL_CreateTextureFromSurface(rend, tmpsurf);
                    SDL_FreeSurface(tmpsurf);
                }
                else {
                    printf("ERROR: Failed to load bmp: %s\n", filebuf);
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
    
    if (NumPickups == 0) {
        printf("ERROR: Failed to read any pickup data. %d items total read.\n", read);
    }
    fclose(file);
    return NumPickups;
}


Pickup PCK_GetWeightedPickup(int x, int y, int active) {
    if (!AllPickups) {
        Pickup product;
        product.x = 0;
        product.y = 0;
        product.ID = 0;
        product.frequency = 0;
        product.file[0] = '\0';
        product.name[0] = '\0';
        product.insulinmod = 0;
        product.watermod = 0;
        product.CHOmod = 0;
        product.caloriesmod = 0;
        product.alcoholmod = 0;
        product.active = 0;
        product.tex = NULL;
        return product;
    }
    
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
    
    Pickup product = AllPickups[index - 1];
    product.x = x;
    product.y = y;
    product.active = active;
    
    return product;
}

Pickup PCK_GetUnweightedPickup(int x, int y, int active) {
    if (!AllPickups) {
        Pickup product;
        product.x = 0;
        product.y = 0;
        product.ID = 0;
        product.frequency = 0;
        product.file[0] = '\0';
        product.name[0] = '\0';
        product.insulinmod = 0;
        product.watermod = 0;
        product.CHOmod = 0;
        product.caloriesmod = 0;
        product.alcoholmod = 0;
        product.active = 0;
        product.tex = NULL;
        return product;
    }
    
    Pickup product = AllPickups[xorshiftplus_uniform(NumPickups)];
    product.x = x;
    product.y = y;
    product.active = active;
    return product;
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





