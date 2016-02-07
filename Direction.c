/* 
 
 Direction.c
 
 Contains the implementations of each function in Direction.h
 
 Last revision: 02/05/16 - Jesse Pritchard
 
*/

#include "Direction.h"


int DIR_ContainsDirection(DirectionMask mask, Direction dir) {
    if (mask & (1 << (int)dir)) {
        return 1;
    }
    else return 0;
}



void DIR_AddDirection(DirectionMask* mask, Direction dir) {
    // Sanity check
    if (!mask) {
        return;
    }
    
    *mask |= (1 << (int)dir);
}

void DIR_MoveCoord(int* x, int* y, Direction dir, int distance) {
    switch (dir) {
        case DIR_LEFT:
            (*x) -= distance;
            break;
            
        case DIR_RIGHT:
            (*x) += distance;
            break;
            
        case DIR_UP:
            (*y) -= distance;
            break;
            
        case DIR_DOWN:
            (*y) += distance;
            break;
    }
}

Direction DIR_GetOpposite(Direction dir) {
    switch (dir) {
        case DIR_LEFT:
            return DIR_RIGHT;
        
        case DIR_RIGHT:
            return DIR_LEFT;
            
        case DIR_UP:
            return DIR_DOWN;
            
        case DIR_DOWN:
            return DIR_UP;
    }
}

Direction DIR_GetClockwise(Direction dir) {
    switch (dir) {
        case DIR_LEFT:
            return DIR_UP;
            
        case DIR_RIGHT:
            return DIR_DOWN;
            
        case DIR_UP:
            return DIR_RIGHT;
            
        case DIR_DOWN:
            return DIR_LEFT;
    }
}

Direction DIR_GetCounterClockwise(Direction dir) {
    switch (dir) {
        case DIR_LEFT:
            return DIR_DOWN;
            
        case DIR_RIGHT:
            return DIR_UP;
            
        case DIR_UP:
            return DIR_LEFT;
            
        case DIR_DOWN:
            return DIR_RIGHT;
    }
}