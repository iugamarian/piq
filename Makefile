all: main

main:
	gcc src/main.c -o bin/main -lbcm2835 -I./include
