all: program

program: main.o gamefs.o parse.o
	gcc -Wall -std=c99 main.o gamefs.o parse.o -o program -lncurses

main.o: src/main.c
	gcc -Wall -std=c99 -c -lncurses src/main.c -Iinclude

gamefs.o: src/gamefs.c
	gcc -Wall -std=c99 -c -lncurses src/gamefs.c -Iinclude

parse.o: src/parse.c
	gcc -Wall -std=c99 -c -lncurses src/parse.c -Iinclude

clean:
	rm *.o program