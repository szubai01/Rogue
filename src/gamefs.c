//Include files
#include "room.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printRec(int yCoord, int xCoord, int height, int width) {
    //Print top line
    for (int i = xCoord; i <= width + xCoord + 1; i++) {
        mvaddch(yCoord, i, '-');
    }

    //Print middle sections
    for (int i = yCoord + 1; i <= height + yCoord + 1; i++) {
        mvaddch(i, xCoord, '|');
        for (int j = xCoord + 1; j <= xCoord + width; j++)
        {
            mvaddch(i, j, '.');
        }
        mvaddch(i, width + xCoord + 1, '|');
    }

    //Print bottom line
    for (int i = xCoord; i <= width + xCoord + 1; i++) {
        mvaddch(height + yCoord + 1, i, '-');
    }
}

void printMsg(int xLoc, int yLoc, char * toPrint)
{
    int tempX, tempY;
    tempY = getcury(stdscr);
    tempX = getcurx(stdscr);
    mvprintw(yLoc, xLoc, toPrint); //Print string
    move(tempY, tempX);
}

void cleanup(Hero * player, Monster ** monList, int nMons, Room ** rooms)
{
    for (int i=0; i < nMons; i++)
    {
        free(monList[i]);
    }
    for (int i=0; i < player->bagC; i++)
    {
        free(player->toolL[i]);
    }
    for (int i=0; i<6; i++)
    {
        free(rooms[i]);
    }
    free(player);
}

void giveDamage(Hero * player, Monster * monster)
{
    char tString[50];

    monster->health = monster->health - player->attackC;
    sprintf(tString, "Hero caused %d damage                            ", player->attackC); //Inform the user
    printMsg(2,0, tString);
    if(monster->health<=0) //If moster died
    {
        sprintf(tString, "Hero killed a %c monster                        ", monster->monType);
        printMsg(2,0, tString);
        printMsg(monster->xPos, monster->yPos, ".");
        monster->monType = '.';
    }
}

int takeDamage(Hero * player, Monster * monster)
{
    if (monster->monType=='A') //If it was an aquator
    {
        if (player->attackC < monster->attack) //If player attack is too low to lose more
        {
            printMsg(2,0, "Too low attack power to lose any               ");
        }
        else
        {
            char tString[50];
            player->attackC = player->attackC - monster->attack; //lose attack power
            sprintf(tString, "Hero lost %d attack power                       ", monster->attack);
            printMsg(2,0, tString);
        }
    }
    else
    {
        char tString[50];
        player->healthC = player->healthC - monster->attack;
        sprintf(tString, "Hero lost %d health                           ", monster->attack); //Lose health
        printMsg(2,0, tString);
        if ((player->healthC) < 1) //If player has died
        {
            return -1;
        }
    }
    return 0;
}

int combat(Hero * player, Monster * monster)
{
    int rNumber;
    rNumber = rand() % 10 + 1; //number to pick who attacks

    if (rNumber > monster->speed)//Hero attacks
    {
        giveDamage(player, monster);
    }
    else
    {
        return takeDamage(player, monster);
    }
    return 0;
}

int moveHero(char inp,  int currX, int currY, char * oldTile, Hero * player, Monster ** monList)
{
    int newX = currX;
    int newY = currY;
    char item;
    int rVal = 0, moved = 0;

    //adjust coordinates according to input character
    switch (inp)
    {
        case 'a':
            newX--;
            break;
        case 'd':
            newX++;
            break;
        case 'w':
            newY--;
            break;
        case 's':
            newY++;
            break;
        default:
            return 0;
    }

    //Checking character at new location
    item = mvinch(newY, newX);
    move(currY, currX);

    switch (item)
    {
        case '+':
        case '#':
        case '.':
        case '!':
        case ']':
        case ')':
        case '(':
            mvaddch(currY, currX, * oldTile);
            mvaddch(newY, newX, '@');
            move(newY, newX);
            rVal = item;
            moved = 1;
            break;
        case '<':
        case '>': //Cases where moving ends game
            return -1;
        //Cases with money involved
        case '8':
        case '*':
            mvaddch(currY, currX, '.');
            mvaddch(newY, newX, '@');
            move(newY, newX);
            rVal = item;
            moved = 1; //Moved is true
            break;
        case 'T':
        case 'A':
        case 'B':
        case 'S':
        case 'Z': //Cases that will cause combat
            rVal = item;
            break;
        default:
            break;
    }

    if ((item=='#') || (item=='+'))
    {
        * oldTile = item;
    }
    else if (moved)
    {
        * oldTile = '.';
    }
    return rVal;
}

int moveMonsters(Monster ** monList, int nMons, int index, Hero * player)
{
    int oldX, oldY, rVal;
    getyx(stdscr, oldY, oldX);
    rVal=0;
    for (int i=0; i<nMons; i++) //Loop through each monster
    {
        int newX, newY;
        char toDo, newTile;
        if (monList[i]->monType=='.') //If monster already died
        {
            continue;
        }
        toDo = monList[i]->pattern[index];  //finds direction to go
        newX = monList[i]->xPos;
        newY = monList[i]->yPos;
        if (toDo == 'r') //if movement is random
        {
            char options[6];
            strcpy(options, "asdw");
            toDo = options[rand() % 4]; //selects a direction
        }
        if (toDo == 'w') //if going up
        {
            newY--;
        }
        else if (toDo == 's') //going down
        {
            newY++;
        }
        else if (toDo == 'a') //going left
        {
            newX--;
        }
        else if (toDo == 'd') //going right
        {
            newX++;
        }
        newTile = mvinch(newY, newX);
        if (newTile == '.') //checks if new tile is a floor, moves there
        {
            mvprintw(monList[i]->yPos, monList[i]->xPos, ".");
            mvaddch(newY, newX, monList[i]->monType);
            monList[i]->xPos = newX;
            monList[i]->yPos = newY;
        }
        else if (newTile == '@') // if new tile is hero, attack time
        {
            rVal = combat(player, monList[i]);
        }
    }
    move(oldY, oldX); //returns cursor to original place
    return rVal;
}

void gameLoop(Room ** rooms, int initX, int initY, int roomNum, int * goldCount, Hero * player, Monster ** monstersList, int numMon)
{
    int potC, bagC, turnCount;
    char input, stringPrint[50];
    char tile = '.';

    potC = bagC = turnCount = 0;

    printMsg(2, 0, "                                          ");
    sprintf(stringPrint, "Health: %d, Potions: %d, Attack: %d, Inv: %d/5", player->healthC, potC, player->attackC, bagC);
    printMsg(2, 56, stringPrint);

    input = getch();

    while (input != 'q') //Loop until game is over via pressing 'q'
    {
        int result, monResult;
        printMsg(2, 0, "                                            ");
        if(input == 'p') //if player desires a potion
        {
            char tPrint[50];
            if (potC > 0) //if there is a potion to use
            {
                player->healthC = 50;
                potC--;
                strcpy(tPrint, "Used a potion, recovered full health     ");
            }
            else //out of potions
            {
                strcpy(tPrint, "Out of Potions            ");
            }
            printMsg(2,0, tPrint);
            sprintf(stringPrint, "Health: %d, Potions: %d, Attack: %d, Inv: %d/5    ", player->healthC, potC, player->attackC, bagC); //updates status bar
            printMsg(2, 56, stringPrint);
            input = getch();
            continue;
        }

        result = moveHero(input, getcurx(stdscr), getcury(stdscr), &tile, player, monstersList);  //moves hero

        if(result == '+') //if moved to a door
        {
            printMsg(2, 0, "Opened a door                              ");
        }
        else if ((result == 'A') || (result == 'B') || (result == 'S') || (result == 'Z') || (result == 'T')) //if player poked a monster
        {
            Monster * locOfMonster = NULL;
            int newX, newY;
            getyx(stdscr, newY, newX);
            if (input == 'w')
            {
                newY--;
            }
            else if (input == 's')
            {
                newY++;
            }
            else if (input == 'a')
            {
                newX--;
            }
            else if (input == 'd')
            {
                newX++;
            }
            for (int i=0; i<numMon; i++)
            {
                if ((monstersList[i]->xPos == newX) && (monstersList[i]->yPos == newY))
                {
                    locOfMonster = monstersList[i];
                    break;
                }
            }
            if(locOfMonster!=NULL)
            {
                int cResult;
                cResult = combat(player, locOfMonster); //Calls combat function
                if (cResult == -1) //if player died
                {
                    break;
                }
            }
        }
        else if ((result == '!') || (result == '*') || (result == '8') || (result == '(') || (result == ')') || (result == ']')) // if some collectable
        {
            if (result == '!') //potion
            {
                printMsg(2, 0, "Picked a potion                           ");
                potC++;
            }
            else if (result== '*') //If money is involved
            {
                char tempStr[50];
                int gPickUp = rand() % 50;
                sprintf(tempStr, "Hero picked up %d gold                      ", gPickUp);
                printMsg(2,0, tempStr);
               * goldCount += gPickUp; //Finds a random num in appropriate range using some number theory
            }
            else if (result == '8')
            {
                char tempStr[50];
                int gPickUp = rand() % 200 + 50;
                sprintf(tempStr, "Hero picked up %d gold             ", gPickUp);
                printMsg(2,0, tempStr);
                * goldCount += gPickUp; //Finds a random num in appropriate range using some number theory
            }
            else
            {
                if (bagC < 5) //if there is room in the bag
                {
                    char tempStr[50];
                    int tVal;
                    sprintf(tempStr, "Hero picked up %c item               ", result);
                    printMsg(2,0, tempStr);
                    player->toolL[bagC] = malloc(sizeof(Tool)); //mallocs for a tool
                    if (player->toolL[bagC] == NULL)
                    {
                        fprintf(stderr, "Out of memory\n");
                        free(player);
                        exit (0);
                    }
                    player->toolL[bagC]->toolType=result;
                    if (result==')')
                    {
                        tVal = rand() % 9 + 1;
                    }
                    if (result=='(')
                    {
                        tVal = rand() % 6 + 10;
                    }
                    if (result==']')
                    {
                        tVal = rand() % 21;
                    }
                    player->toolL[bagC]->toolVal = tVal;
                    bagC++;
                    player->bagC = bagC;
                }
                else //if bag is full
                {
                    char tempStr[50];
                    sprintf(tempStr, "Hero walked on %c and broke it        ", result);
                    printMsg(2,0, tempStr);
                }
            }
        }
        else if (result == -1) //If stairs were encountered
        {
            break;
        }

        monResult = moveMonsters(monstersList, numMon, turnCount % 4, player);
        if (monResult == -1)
        {
            break;
        }

        sprintf(stringPrint, "Health: %d, Potions: %d, Attack: %d, Inv: %d/5    ", player->healthC, potC, player->attackC, bagC);
        printMsg(2, 56, stringPrint);
        turnCount++;

        input = getch(); //gets another char from the user
    }
}




void drawRoom(Room * dRoom, int xStart, int yStart, int * currX, int * currY, Monster ** monstersList, int * mCount)
{
    printRec(yStart, xStart, dRoom->height, dRoom->width); //draws borders

    //Drawing doors + hallways
    for (int i=0; i < dRoom->numDoors; i++)
    {
        char side = dRoom->doorSide[i];
        switch (side)
        {
            case 'n':
                mvaddch(yStart, xStart + dRoom->doorLocation[i] + 1, '+');
                mvaddch(yStart - 1, xStart + dRoom->doorLocation[i] + 1, '#');
                for (int j = xStart + dRoom->doorLocation[i] + 1; j > xStart - 2; j--)
                {
                    mvaddch(yStart - 2, j, '#');
                }
                break;
            case 'w':
                mvaddch(yStart + dRoom->doorLocation[i] + 1, xStart, '+');
                mvaddch(yStart + dRoom->doorLocation[i] + 1, xStart - 1, '#');
                mvaddch(yStart + dRoom->doorLocation[i] + 1, xStart - 2, '#');
                break;
            case 'e':
                mvaddch(yStart + dRoom->doorLocation[i] + 1, xStart + dRoom->width + 1, '+');
                for (int j = 2; j <= 25 - dRoom->width; j++)
                {
                    mvaddch(yStart + dRoom->doorLocation[i] + 1, xStart + dRoom->width + j, '#');
                }
                break;
            case 's':
                mvaddch(yStart + dRoom->height + 1, xStart + dRoom->doorLocation[i] + 1, '+');
                mvaddch(yStart + dRoom->height + 2, xStart + dRoom->doorLocation[i] + 1, '#');
                for (int j = xStart + dRoom->doorLocation[i] + 1; j < (26 + xStart); j++)
                {
                    mvaddch(yStart + dRoom->height + 3, j, '#');
                }
                break;
        }
    }

    //Drawing items
    for (int i=0; i < dRoom->numItems; i++)
    {
        char typeChar, displayChar;
        typeChar = dRoom->item[i];
        switch (typeChar)
        {
            case 'h':
                displayChar = '@';
                * currY = yStart + dRoom->itemY[i] + 1;
                * currX = xStart + dRoom->itemX[i] + 1;
                break;
            case 'z':
                displayChar = '<';
                break;
            case 'a':
                displayChar = '>';
                break;
            case 'g':
                displayChar = '*';
                break;
            case 'G':
                displayChar = '8';
                break;
            case 'w':
                displayChar = ')';
                break;
            case 'W':
                displayChar = '(';
                break;
            case 'm': ; //Not a typo, empty statement needed prior to declaration
                char monsters[6];
                strcpy(monsters, "ABSZ");
                int ranNum = rand() % 4;
                int mCt = * mCount;
                displayChar = monsters[ranNum];
                monstersList[mCt]->monType = displayChar;
                monstersList[mCt]->xPos = xStart + dRoom->itemX[i] + 1;
                monstersList[mCt]->yPos = yStart + dRoom->itemY[i] + 1;
                if (displayChar == 'A')
                {
                    monstersList[mCt]->health = 5;
                    monstersList[mCt]->attack = 1;
                    monstersList[mCt]->speed = 2;
                    strcpy(monstersList[mCt]->pattern, "nnnn");
                }
                if (displayChar == 'B')
                {
                    monstersList[mCt]->health = 2;
                    monstersList[mCt]->attack = 5;
                    monstersList[mCt]->speed = 4;
                    strcpy(monstersList[mCt]->pattern, "rrrr");
                }
                if (displayChar == 'S')
                {
                    monstersList[mCt]->health = 5;
                    monstersList[mCt]->attack = 5;
                    monstersList[mCt]->speed = 4;
                    strcpy(monstersList[mCt]->pattern, "ddaa");
                }
                if (displayChar == 'Z')
                {
                    monstersList[mCt]->health = 15;
                    monstersList[mCt]->attack = 5;
                    monstersList[mCt]->speed = 2;
                    strcpy(monstersList[mCt]->pattern, "wsws");
                }
                * mCount = * mCount + 1;
                break;
            case 'e':
                displayChar = ']';
                break;
            case 'M':
                mCt = * mCount;
                displayChar = 'T';
                monstersList[mCt]->monType = displayChar;
                monstersList[mCt]->xPos = xStart + dRoom->itemX[i] + 1;
                monstersList[mCt]->yPos = yStart + dRoom->itemY[i] + 1;
                monstersList[mCt]->health = 50;
                monstersList[mCt]->attack = 5;
                monstersList[mCt]->speed = 3;
                strcpy(monstersList[mCt]->pattern, "wdsa");
                * mCount = * mCount + 1;
                break;
            case 'p':
                displayChar = '!';
                break;
        }
        mvaddch(yStart + dRoom->itemY[i] + 1, xStart + dRoom->itemX[i] + 1, displayChar);
    }
}

void startGame(Room * rooms[6], int * goldT, int numOfMons)
{
    int maxY, maxX;
    int currY=-1, currX=-1;
    int roomNum=0;
    int monCount = 0;
    Monster * monsters[numOfMons];

    Hero * mainHero;
    mainHero = malloc(sizeof(Hero)); //Mallocs for hero
    for (int i = 0; i < numOfMons; i++)
    {
        monsters[i] = malloc(sizeof(Monster));
        if (monsters[i] == NULL)
        {
            free(mainHero);
            break;
        }
    }
    if (mainHero == NULL) //If memory issues
    {
        fprintf(stderr, "Out of memory.");

        endwin();
        for (int i=0; i<6; i++)
        {
            free(rooms[i]);
        }

        exit(0);
    }
    //Sets hero's starter details
    mainHero->healthC = 50;
    mainHero->attackC = 5;
    mainHero->bagC = 0;
    mainHero->potC = 0;

    //Setting things
    srand(time(NULL));
    initscr();
    noecho();
    cbreak();
    getmaxyx(stdscr, maxY, maxX);

    //check screen size
    if ((maxY < 57) || (maxX < 87))
    {
        mvprintw(0,0, "Screen too small");
        mvprintw(1,0, "Need 81x53 (XxY)");
        mvprintw(2,0, "Have %dx%d", maxX, maxY);
        getch();
        endwin();
        exit(0);
    }

    //Print major hallways
    for (int i=1; i<86;i++)
    {
        mvprintw(28, i, "#");
    }
    for (int i=4; i<53; i++)
    {
        mvprintw(i, 1, "#");
        mvprintw(i, 29, "#");
        mvprintw(i, 57, "#");
        mvprintw(i, 85, "#");
    }

    //Draw rooms, by vertical order
    for (int i=0; i<3; i++)
    {
        drawRoom(rooms[i], i * 28 + 3, 6, &currX, &currY, monsters, &monCount);
        if ((currX!=-1) && (currY!=-1) && (roomNum==0))
        {
            roomNum = i + 1;
        }

        drawRoom(rooms[i+3], i * 28 + 3, 31, &currX, &currY, monsters, &monCount);
        if ((currX!=-1) && (currY!=-1) && (roomNum==0))
        {
            roomNum = i + 4;
        }
    }

    //Lets go
    move(currY, currX);
    gameLoop(rooms, currX, currY, roomNum, goldT, mainHero, monsters, numOfMons);

    //End ncurses
    endwin();

    //Print tools and free them
    printf("Items picked up: \n");
    for (int i=0; i < mainHero->bagC; i++)
    {
        printf("Type %c, value %d\n", mainHero->toolL[i]->toolType, mainHero->toolL[i]->toolVal);
    }

    //Freedom for more memory
    cleanup(mainHero, monsters, monCount, rooms);

    printf("Game terminated. You earned %d Gold!\n", * goldT);
}

