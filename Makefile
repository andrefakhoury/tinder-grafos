all:
	gcc src/adjMatrix.c src/tinder.c src/main.c src/profile.c -I include/ -o main -Wall -std=c99 -g -lm

run:
	./main

debug:
	gdb ./main

valgrind:
	valgrind --leak-check=full -v ./main