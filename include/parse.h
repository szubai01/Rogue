#ifndef __SZUBAI01_PARSE__
#define __SZUBAI01_PARSE__

/*
parseLine
Parses line provide to it, storing data in structure
IN: string to parse and pointer to a Room, and pointer to number of monsters
OUT: None
POST: Saves data into structure
*/
void parseLine (char *, Room *, int *);

/*
initParse
Begins to parse the level file that was provided
IN: array to pointer of Room structure and file name and pointer to number of monsters
OUT: None
POST: Allocates memory for first arguement, saves data to structure via parseLine function
*/
void initParse(Room **, char *, int *);

#endif
