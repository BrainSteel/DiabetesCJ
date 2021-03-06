#include "Level.h"
#include "xorshift.h"
#include "math.h"

static int LVL_isConnected(Wall one, Wall two) {
    if (abs(one.x - two.x) > 1 || abs(one.y - two.y) > 1) {
        return 0;
    }
    
    int ccw_check = 1;
    int cw_check = 2;
    
    if (one.dir == DIR_LEFT || one.dir == DIR_UP) {
        DIR_MoveCoord(&one.x, &one.y, one.dir, 1);
        one.dir = DIR_GetOpposite(one.dir);
        
        int tmp = cw_check;
        cw_check = ccw_check;
        ccw_check = tmp;
    }
    
    if (two.dir == DIR_LEFT || two.dir == DIR_UP) {
        DIR_MoveCoord(&two.x, &two.y, two.dir, 1);
        two.dir = DIR_GetOpposite(two.dir);
    }
    
    if (one.dir == two.dir) {
        Direction ccw = DIR_GetCounterClockwise(one.dir);
        int x = one.x, y = one.y;
        DIR_MoveCoord(&x, &y, ccw, 1);
        if (x == two.x && y == two.y) {
            return ccw_check;
        }
        
        
        Direction cw = DIR_GetClockwise(one.dir);
        x = one.x;
        y = one.y;
        DIR_MoveCoord(&x, &y, cw, 1);
        if (x == two.x && y == two.y) {
            return cw_check;
        }
        
    }
    else {
        if (one.dir == DIR_DOWN) {
            Direction cw = DIR_GetClockwise(one.dir);
            int x = one.x, y = one.y;
            DIR_MoveCoord(&x, &y, cw, 1);
            if (x == two.x && y == two.y) {
                return cw_check;
            }
            DIR_MoveCoord(&x, &y, one.dir, 1);
            if (x == two.x && y == two.y) {
                return cw_check;
            }
        
        
            if (one.x == two.x && one.y == two.y) {
                return ccw_check;
            }
            x = one.x;
            y = one.y;
            DIR_MoveCoord(&x, &y, one.dir, 1);
            if (x == two.x && y == one.y) {
                return ccw_check;
            }
        }
        else {
            Direction ccw = DIR_GetCounterClockwise(one.dir);
            int x = one.x, y = one.y;
            DIR_MoveCoord(&x, &y, ccw, 1);
            if (x == two.x && y == two.y) {
                return ccw_check;
            }
            DIR_MoveCoord(&x, &y, one.dir, 1);
            if (x == two.x && y == two.y) {
                return ccw_check;
            }
            
            
            if (one.x == two.x && one.y == two.y) {
                return cw_check;
            }
            x = one.x;
            y = one.y;
            DIR_MoveCoord(&x, &y, one.dir, 1);
            if (x == two.x && y == one.y) {
                return cw_check;
            }
        }
    }
    
    return 0;
}

int LVL_AddWall(Level* lvl, Wall wall, Wall* from) {
    
    Cell cell = LVL_GetCell(lvl, wall.x, wall.y);
    if (DIR_ContainsDirection(cell.mask, wall.dir)) {
        return 0;
    }
    
    
    if (LVL_GetNumWalls(cell) >= 2) {
        return 0;
    }
    
    int acrossx = wall.x, acrossy = wall.y;
    DIR_MoveCoord(&acrossx, &acrossy, wall.dir, 1);
    Cell across = LVL_GetCell(lvl, acrossx, acrossy);
    
    if (DIR_ContainsDirection(across.mask, DIR_GetOpposite(wall.dir))) {
        return 0;
    }
    
    if (LVL_GetNumWalls(across) >= 2) {
        return 0;
    }
    
    Direction cw = DIR_GetClockwise(wall.dir);
    Direction ccw = DIR_GetCounterClockwise(wall.dir);
    
    if (from) {
        int choice = LVL_isConnected(wall, *from);
        if (!choice) {
            return 0;
        }
        else if (choice == 1) {
            // Check cw for connections
            if (DIR_ContainsDirection(cell.mask, cw)) {
                return 0;
            }
            int x = cell.x, y = cell.y;
            DIR_MoveCoord(&x, &y, wall.dir, 1);
            DIR_MoveCoord(&x, &y, cw, 1);
            Cell cwcell = LVL_GetCell(lvl, x, y);
            if (DIR_ContainsDirection(cwcell.mask, ccw)) {
                return 0;
            }
        }
        else {
            // Check ccw for connections
            if (DIR_ContainsDirection(cell.mask, ccw)) {
                return 0;
            }
            int x = cell.x, y = cell.y;
            DIR_MoveCoord(&x, &y, wall.dir, 1);
            DIR_MoveCoord(&x, &y, ccw, 1);
            Cell ccwcell = LVL_GetCell(lvl, x, y);
            if (DIR_ContainsDirection(ccwcell.mask, cw)) {
                return 0;
            }

        }
    }
    else {
        // Check cw for connections
        if (DIR_ContainsDirection(cell.mask, cw)) {
            return 0;
        }
        int x = cell.x, y = cell.y;
        DIR_MoveCoord(&x, &y, wall.dir, 1);
        DIR_MoveCoord(&x, &y, cw, 1);
        Cell cwcell = LVL_GetCell(lvl, x, y);
        if (DIR_ContainsDirection(cwcell.mask, ccw)) {
            return 0;
        }
        
        // Check ccw for connections
        if (DIR_ContainsDirection(cell.mask, ccw)) {
            return 0;
        }
        x = cell.x, y = cell.y;
        DIR_MoveCoord(&x, &y, wall.dir, 1);
        DIR_MoveCoord(&x, &y, ccw, 1);
        Cell ccwcell = LVL_GetCell(lvl, x, y);
        if (DIR_ContainsDirection(ccwcell.mask, cw)) {
            return 0;
        }
    }
    
    // Set the wall
    Cell* cptr = LVL_GetCellPtr(lvl, wall.x, wall.y);
    DIR_AddDirection(&cptr->mask, wall.dir);
    DIR_MoveCoord(&wall.x, &wall.y, wall.dir, 1);
    cptr = LVL_GetCellPtr(lvl, wall.x, wall.y);
    DIR_AddDirection(&cptr->mask, DIR_GetOpposite(wall.dir));
    return 1;
}

static void LVL_CreateQuadrantLayerPaths(Level* lvl, int layer) {
    if (layer <= 0) {
        return;
    }
    
    int halfwidth = LVL_WIDTH / 2;
    int halfheight = LVL_HEIGHT / 2;
    
#define LAYER_CHANCE 0.2
#define STRAIGHT_CHANCE 0.5
#define LAYER_LENGTH 8
    
    Wall wall;
    wall.y = layer - 1;
    wall.dir = DIR_DOWN;
    
    Wall prev;
    int length = 0;
    int success = 0;
    for (wall.x = halfwidth - 1; wall.x >= layer; wall.x--) {
        uint64_t rnd = xorshiftplus();
        if (rnd <= (LAYER_CHANCE * UINT64_MAX)) {
            wall.dir = DIR_DOWN;
            success = LVL_AddWall(lvl, wall, NULL);
            if (success) {
                for (length++; length < LAYER_LENGTH && success && wall.x >= layer && wall.y < halfheight; length++) {
                    prev = wall;
                    rnd = xorshiftplus();
                    if (rnd <= (STRAIGHT_CHANCE) * UINT64_MAX) {
                        wall.x--;
                        wall.dir = DIR_DOWN;
                    }
                    else {
                        wall.y++;
                        wall.dir = DIR_LEFT;
                    }
                    if (wall.x >= layer && wall.y < halfheight) {
                        success = LVL_AddWall(lvl, wall, &prev);
                    }
                    else success = 0;
                }
                wall.y = layer - 1;
                if (!success) {
                    length = 0;
                }
            }
        }
    }
    length = 0;
    wall.x = layer - 1;
    wall.dir = DIR_RIGHT;
    for (wall.y = layer; wall.y < halfheight; wall.y++) {
        uint64_t rnd = xorshiftplus();
        if (rnd <= (LAYER_CHANCE * UINT64_MAX)) {
            wall.dir = DIR_RIGHT;
            
            /* If we just successfully put a wall in, try to add to it */
            if (success) {
                prev = wall;
                success = LVL_AddWall(lvl, wall, &prev);
            }
            else {
                success = LVL_AddWall(lvl, wall, NULL);
            }
            
            if (success) {
                for (length++; length < LAYER_LENGTH && success && wall.y < halfheight && wall.x < halfwidth; length++) {
                    prev = wall;
                    rnd = xorshiftplus();
                    if (rnd <= (STRAIGHT_CHANCE) * UINT64_MAX) {
                        wall.y++;
                        wall.dir = DIR_RIGHT;
                    }
                    else {
                        wall.x++;
                        wall.dir = DIR_DOWN;
                    }
                    if (wall.y < halfheight && wall.x < halfwidth) {
                        success = LVL_AddWall(lvl, wall, &prev);
                    }
                    else success = 0;
                }
                wall.x = layer - 1;
                if (!success) {
                    length = 0;
                }
            }
        }
    }
}

Level* LVL_Generate(int day, int totalwidth, int totalheight) {

    Level* lvl = malloc(sizeof(Level));
    if (!lvl) return NULL;
    
    
    lvl->width = LVL_WIDTH;
    lvl->height = LVL_HEIGHT;
    lvl->totwidth = totalwidth;
    lvl->totheight = totalheight;
    lvl->cellwidth = totalwidth / lvl->width;
    lvl->cellheight = totalheight / lvl->height;
    lvl->num = lvl->width * lvl->height;
    lvl->grid = malloc(sizeof(Cell) * lvl->num);
    if (!lvl->grid) return NULL;
    
    int halfwidth = LVL_WIDTH / 2;
    int halfheight = LVL_HEIGHT / 2;
    
    int count;
    for (count = 0; count < lvl->num; count++) {
        lvl->grid[count].x = count % lvl->width;
        lvl->grid[count].y = count / lvl->width;
        lvl->grid[count].mask = 0;
        lvl->grid[count].item = NULL;
        
        if (lvl->grid[count].x == 0) {
            LVL_AddToCellMask(lvl, lvl->grid[count].x, lvl->grid[count].y, DIR_LEFT);
        }
        else if (lvl->grid[count].x == lvl->width - 1) {
            LVL_AddToCellMask(lvl, lvl->grid[count].x, lvl->grid[count].y, DIR_RIGHT);
        }
        
        if (lvl->grid[count].y == 0) {
            LVL_AddToCellMask(lvl, lvl->grid[count].x, lvl->grid[count].y, DIR_UP);
        }
        else if (lvl->grid[count].y == lvl->height - 1) {
            LVL_AddToCellMask(lvl, lvl->grid[count].x, lvl->grid[count].y, DIR_DOWN);
        }
    }
    
    int layer;
    for (layer = 1; layer < halfwidth; layer++) {
        LVL_CreateQuadrantLayerPaths(lvl, layer);
    }

    // Mirror the first quadrant onto the right quadrant
    int xpos, ypos;
    for (ypos = 0; ypos < halfheight; ypos++) {
        for (xpos = halfwidth; xpos < lvl->width; xpos++) {
            Cell original = LVL_GetCell(lvl, lvl->width - xpos - 1, ypos);
            
            // Copy the vertical directions
            if (DIR_ContainsDirection(original.mask, DIR_UP)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_UP);
            }
            
            if (DIR_ContainsDirection(original.mask, DIR_DOWN)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_DOWN);
            }
            
            // Mirror the horizontal directions
            if (DIR_ContainsDirection(original.mask, DIR_LEFT)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_RIGHT);
            }
            
            if (DIR_ContainsDirection(original.mask, DIR_RIGHT)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_LEFT);
            }
        }
    }
    
    // Mirror the top half onto the bottom half
    for (ypos = halfheight; ypos < lvl->height; ypos++) {
        for (xpos = 0; xpos < lvl->width; xpos++) {
            Cell original = LVL_GetCell(lvl, xpos, lvl->height - ypos - 1);
            
            // Copy the horizontal directions
            if (DIR_ContainsDirection(original.mask, DIR_LEFT)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_LEFT);
            }
            
            if (DIR_ContainsDirection(original.mask, DIR_RIGHT)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_RIGHT);
            }
            
            // Mirror the vertical directions
            if (DIR_ContainsDirection(original.mask, DIR_UP)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_DOWN);
            }
            
            if (DIR_ContainsDirection(original.mask, DIR_DOWN)) {
                LVL_AddToCellMask(lvl, xpos, ypos, DIR_UP);
            }
        }
    }
    
    int pickupw = lvl->width - 2;
    int pickuph = lvl->height - 2;
    lvl->numpickup = 4 * ((int)(LVL_PERCENT_PICKUPS * pickupw * pickuph) / 4);
    lvl->pickups = malloc(sizeof(Pickup) * lvl->numpickup);
    if (!lvl->pickups) {
        LVL_DestroyLevel(lvl);
        return NULL;
    }
    
    int pos[lvl->numpickup / 4][2];
    
    for (count = 0; count < lvl->numpickup; count += 4) {
        int xpos = (int)xorshiftplus_uniform(pickupw / 2) + 1;
        int ypos = (int)xorshiftplus_uniform(pickuph / 2) + 1;
        int samecheck;
        int reset = 0;
        for (samecheck = 0; samecheck < count / 4; samecheck++) {
            if (pos[samecheck][0] == xpos && pos[samecheck][1] == ypos) {
                reset = 1;
                break;
            }
        }
        if (reset) {
            count-=4;
            continue;
        }
        pos[count / 4][0] = xpos;
        pos[count / 4][1] = ypos;
        
        Cell* cptr = NULL;
        lvl->pickups[count] = PCK_GetWeightedPickup(xpos, ypos, 1);
        cptr = LVL_GetCellPtr(lvl, xpos, ypos);
        cptr->item = &lvl->pickups[count];
        
        lvl->pickups[count + 1] = PCK_GetWeightedPickup(lvl->width - xpos - 1, ypos, 1);
        cptr = LVL_GetCellPtr(lvl, lvl->width - xpos - 1, ypos);
        cptr->item = &lvl->pickups[count + 1];
        
        lvl->pickups[count + 2] = PCK_GetWeightedPickup(xpos, lvl->height - ypos - 1, 1);
        cptr = LVL_GetCellPtr(lvl, xpos, lvl->height - ypos - 1);
        cptr->item = &lvl->pickups[count + 2];
        
        lvl->pickups[count + 3] = PCK_GetWeightedPickup(lvl->width - xpos - 1, lvl->height - ypos - 1, 1);
        cptr = LVL_GetCellPtr(lvl, lvl->width - xpos - 1, lvl->height - ypos - 1);
        cptr->item = &lvl->pickups[count + 3];
    }
    
    return lvl;
}


int LVL_GetNumWalls(Cell cell) {
    int num = 0;
    if (cell.mask & DIR_LEFT_MASK) {
        num++;
    }
    if (cell.mask & DIR_RIGHT_MASK) {
        num++;
    }
    if (cell.mask & DIR_UP_MASK) {
        num++;
    }
    if (cell.mask & DIR_DOWN_MASK) {
        num++;
    }
    return num;
}

Cell LVL_GetCell(Level* lvl, int x, int y) {
    if (!lvl || !lvl->grid) {
        /* This isn't good */
        Cell cell;
        cell.x = -1;
        cell.y = -1;
        cell.mask = 0;
        cell.item = NULL;
        return cell;
    }
    
    return lvl->grid[y * lvl->width + x];
}

Cell* LVL_GetCellPtr(Level* lvl, int x, int y) {
    if (!lvl || !lvl->grid) {
        return NULL;
    }
    
    return &lvl->grid[y * lvl->width + x];
}

void LVL_SetCellMask(Level* lvl, int x, int y, DirectionMask mask) {
    if (!lvl) {
        return;
    }
    
    if (!lvl->grid) {
        return;
    }
    
    lvl->grid[y * lvl->width + x].mask = mask;
}

void LVL_AddToCellMask(Level* lvl, int x, int y, Direction dir) {
    if (!lvl) {
        return;
    }
    
    if (!lvl->grid) {
        return;
    }
    
    DIR_AddDirection(&lvl->grid[y * lvl->width + x].mask, dir);
}

void LVL_DestroyLevel(Level* lvl) {
    if (lvl->grid) {
        free(lvl->grid);
    }
    
    if (lvl->pickups) {
        free(lvl->pickups);
    }
    
    free(lvl);
}