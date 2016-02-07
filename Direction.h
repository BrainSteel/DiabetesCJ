
/* 
 
Direction.h

 General definitions for direction data.
 
 Last Revision: 02/05/2016 9:19 PM - Jesse Pritchard 
 
*/


#ifndef Direction_h
#define Direction_h

typedef unsigned char DirectionMask;

typedef enum eDirection {
    DIR_LEFT = 0,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
} Direction;

#define DIR_LEFT_MASK   ((1 << DIR_LEFT))
#define DIR_RIGHT_MASK  ((1 << DIR_RIGHT))
#define DIR_UP_MASK     ((1 << DIR_UP))
#define DIR_DOWN_MASK   ((1 << DIR_DOWN))

/* 

 DIR_ContainsDirection: Direction.c
 Checks if the given direction is contained in the given mask
 
 DirectionMask mask:
 The mask to check
 
 Direction dir:
 The given direction
 
 Return value:
 1 if dir is in mask, 0 otherwise
 
*/
int DIR_ContainsDirection(DirectionMask mask, Direction dir);

/* 
 DIR_AddDirection: Direction.c
 Adds the given direction to the given mask
 
 DirectionMask* mask:
 The mask to be added to
 
 Direction dir:
 The direction to add
 
 Return value: None
 
 Remarks: If mask == NULL, the function has no effect
*/
void DIR_AddDirection(DirectionMask* mask, Direction dir);

/* 
 DIR_MoveCoord: Direction.c
 Moves the given coordinate once in the given direction
 
 int* x:
 The x-coordinate to be moved
 
 int* y:
 The y-coordinate to be moved
 
 Direction dir:
 The direction to move in

 Return value: None
 
 Remarks:
 Upwards is considered the negative y direction, and left is considered to be the negative x direction

*/
void DIR_MoveCoord(int* x, int* y, Direction dir);

/* 
 DIR_GetOpposite: Direction.c
 Gets the opposite direction of the specified direction
 
 Direction dir:
 The direction to negate
 
 Return value:
 The direction that is opposite dir
 
*/
Direction DIR_GetOpposite(Direction dir);

/*
 DIR_GetOpposite: Direction.c
 Gets the direction located clockwise from the specified direction
 
 Direction dir:
 The base direction
 
 Return value:
 The direction that is clockwise from dir
 
 */
Direction DIR_GetClockwise(Direction dir);


/*
 DIR_GetOpposite: Direction.c
 Gets the direction located counterclockwise from the specified direction
 
 Direction dir:
 The base direction
 
 Return value:
 The direction that is counterclockwise from dir
 
 */
Direction DIR_GetCounterClockwise(Direction dir);


#endif /* Direction_h */
