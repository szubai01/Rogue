#include "room.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VALCHARS "hzagGwWmeMp"

void parseLine (char * string, Room * currRoom, int * nMons)
{
    int len, x, y, doorCount, itemCount;
    char * dPtr;
    len = strlen(string);

    //Replacing X and commas with space for easier parsing
    for (int i=0; i<len; i++)
    {
        if ((string[i] == 'X') || (string[i] == ','))
        {
            string[i] = ' ';
        }
    }

    //Scan and set dimensions of room
    sscanf(string, "%6d%*c%6d", &y, &x);
    if ((y > 18) || (y < 1))
    {
        fprintf(stderr, "Room resized to 18 rows from %d\n", y);
        y=18;
    }
    if ((x > 23) || (x < 1))
    {
        fprintf(stderr, "Room resized to 23 columns from %d\n", x);
        x = 23;
    }
    currRoom->width = x;
    currRoom->height = y;

    //Searches and processes doors
    doorCount=0;
    dPtr = strchr(string, 'd');
    while ((dPtr!= NULL) && (doorCount<4))
    {
        int doorLoc;
        currRoom->doorSide[doorCount] = (char) *(dPtr + 1);

        sscanf(dPtr+2,"%6d", &doorLoc);
        currRoom->doorLocation[doorCount] = doorLoc;
        doorCount ++;
        dPtr = strchr(dPtr + 1, 'd');
    }
    currRoom->numDoors = doorCount;

    itemCount=0;
    dPtr = strpbrk(string, VALCHARS);
    while ((dPtr!= NULL) && (itemCount<10))
    {
        if (*(dPtr-1)=='d') //accounts for doors on east wall
        {
            dPtr = strpbrk(dPtr + 1, VALCHARS);
            continue;
        }
        int xLoc, yLoc;

        sscanf(dPtr+1,"%6d %6d", &yLoc, &xLoc);
        if ((* dPtr == 'm') || (* dPtr == 'M'))
        {
            * nMons = * nMons + 1;
        }
        currRoom->item[itemCount] = (char) *(dPtr);
        currRoom->itemX[itemCount] = xLoc;
        currRoom->itemY[itemCount] = yLoc;
        itemCount ++;
        dPtr = strpbrk(dPtr + 1, VALCHARS);
    }
    currRoom->numItems = itemCount;
}


void initParse(Room * rooms[6], char * fName, int * nMons)
{
    FILE * fPtr;
    fPtr = fopen(fName, "r");

    * nMons = 0;

    if (fPtr == 0)
    {
        fprintf(stderr, "Could not open file\n");
        exit(0);
    }
    char currLine[155];

    for (int i=0; i<6; i++)
    {
        rooms[i] = malloc(sizeof(Room));
        if (rooms[i]==NULL)
        {
            fprintf(stderr, "Unable to malloc, out of memory\n");
            for (int j = i-1; j>=0; j--)
            {
                free(rooms[j]);
            }
            exit(0);
        }
        fgets(currLine, 150, fPtr);
        parseLine(currLine, rooms[i], nMons);
    }
    fclose(fPtr);
}
