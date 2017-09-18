#ifndef __SZUBAI01_ROOM__
#define __SZUBAI01_ROOM__

#include <stdlib.h>

struct RoomStr
{
    int width, height, numItems, numDoors;
    char doorSide[4];
    char item[10];
    int itemX[10], itemY[10], doorLocation[4], itemVal[10];
};

typedef struct RoomStr Room;

struct toolStr
{
    char toolType;
    int toolVal;
};

typedef struct toolStr Tool;

struct monsterStr
{
    int xPos, yPos;
    char monType;
    char pattern[4];
    int health, speed, attack;
};

typedef struct monsterStr Monster;

struct heroStr
{
    int healthC, bagC, potC, attackC;
    Tool * toolL[5];
};

typedef struct heroStr Hero;

#endif
