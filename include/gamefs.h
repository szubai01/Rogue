#ifndef __SZUBAI01_GAMEFS__
#define __SZUBAI01_GAMEFS__


/*
printRec
Prints a rectange with '-' and '|'
Note: originally designed by Dr. Carvalho, used with permission
IN: y and x starting points, height and width of rectange
OUT: None
POST: Rectangle is drawn to screen
*/
void printRec(int, int, int, int);

/*
printMsg
Prints a string
IN: y and x starting points, and the string
OUT: None
POST: Message is printed to screen, without changing original cursor position
*/
void printMsg(int, int, char *);

/*
cleanup
Cleans up memory
IN: Pointers to structs used in the program
OUT: None
POST: All structures are cleaned up
*/
void cleanup(Hero *, Monster **, int, Room **);

/*
giveDamage
Deals damage
IN: Hero pointer and Monster pointer
OUT: None
POST: Damage is given to monster
*/
void giveDamage(Hero * player, Monster * monster);

/*
takeDamage
Recieves damage
IN: Hero pointer and Monster pointer
OUT: -1 if player dies
POST: Damage is given to player
*/
int takeDamage(Hero * player, Monster * monster);

/*
combat
Performs combat
IN: Pointer to a Hero and a Monster
OUT: Result of combat
POST: Hero or Monster loses hp or attack
*/
int combat(Hero *, Monster *);


/*
moveHero
Moves the hero, according to input char and coordinates in X and Y
IN: Character of input, x and y coordinates, character pointer of old tile, hero pointer, monster array pointer
OUT: Result, according to what is in attempted new position
POST: Hero may be moved, collectables are collected if applicable.
*/
int moveHero(char inp,  int currX, int currY, char * oldTile, Hero * player, Monster ** monList);

/*
moveMonsters
Moves the monsters, according to their path and turn number
IN: List of monsters, number of monsters, turn number and player index
OUT: Result, according to what is in attempted new position
POST: Monsters carry on patrol, maybe a combat happens too
*/
int moveMonsters(Monster ** monList, int nMons, int index, Hero * player);

/*
gameLoop
Loop for game, continues while game is active
IN: Array of Room struct pointers, initial x and y, room count, pointer to gold count, pointer of hero, pointer of list of monsters, number of monsters
OUT: None
POST: Game is played until user exits or steps on stairs
*/
void gameLoop(Room **, int, int, int, int *, Hero *, Monster **, int);

/*
drawRoom
Draws the room  given, in the desired location
IN: Room struct pointer to draw, coordinates in x and y of where to draw, and pointers of hero's location
OUT: Hero's location, if specified in room (via pointer)
POST: Draws room, and changes hero's location if needed
*/
void drawRoom(Room *, int, int, int *, int *);

/*
startGame
Starts and finishes the game
IN: Array of Room struct points, pointer to gold count
OUT: Gold count (via pointer)
POST: Game is played
*/
void startGame(Room **, int *, int);

#endif
