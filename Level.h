
#ifndef Level_h
#define Level_h

#include "SDL.h"
#include "Direction.h"
#include "Pickup.h"

#define LVL_WIDTH 40
#define LVL_HEIGHT 40
#define LVL_PERCENT_PICKUPS 0.08

typedef struct sWall {
    int x, y;
    Direction dir;
} Wall;

typedef struct sCell {
    DirectionMask mask;
    int x, y;
    Pickup* item;
} Cell;

typedef struct sLevel {
    int totwidth, totheight;
    int cellwidth, cellheight;
    int width, height;
    int num;
    Cell* grid;

    int day;
    
    Pickup* pickups;
    int numpickup;
    
} Level;

int LVL_AddWall(Level* lvl, Wall wall, Wall* from);
Level* LVL_Generate(int day, int totalwidth, int totalheight);
int LVL_GetNumWalls(Cell cell);
Cell LVL_GetCell(Level* lvl, int x, int y);
Cell* LVL_GetCellPtr(Level* lvl, int x, int y);
void LVL_SetCellMask(Level* lvl, int x, int y, DirectionMask mask);
void LVL_AddToCellMask(Level* lvl, int x, int y, Direction dir);
void LVL_DestroyLevel(Level* lvl);

#endif /* Level_h */
