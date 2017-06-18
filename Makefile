all:
	gcc -o cave4 cave4.c -lcurses
install:
	cp cave4 /usr/games
