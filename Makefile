all:
	gcc src/adjMatrix.c src/tinder.c src/main.c -I include/ -o main -Wall -std=c99 -g

run:
	./main

debug:
	gdb ./main

valgrind:
	valgrind --leak-check=full -v ./main