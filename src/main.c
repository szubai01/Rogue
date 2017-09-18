/******************************************
CIS 2500                          A2: Rogue
Syed Usman Zubair                 2/14/2016
szubai01@mail.uoguelph.ca

Program takes in a level file as a command
line arguement, then launches a game of
Rogue
******************************************/

//Include files
#include "room.h"
#include <stdio.h>
#include "parse.h"
#include "gamefs.h"


/*
main
Program takes a level file as a command line arguement, and then launches a game of Rogue using the
information provide in said file.
IN: filename of level file
OUT: None
POST: Takes control of terminal screen while running
*/
int main (int argc, char * argv[])
{
    //Declare variables
    Room * rooms[6];
    int gold = 0, nMonsters;

    //Checking for appropriate number of args
    if (argc !=2)
    {
        fprintf(stderr, "Incorrect number of arguments entered. Expected 2, recieved %d\n", argc);
        return 0;
    }
    initParse(rooms, argv[1], &nMonsters); //Starts the parsing of the level file
    startGame(rooms, &gold, nMonsters); //Starts the game

    return 0;
}
