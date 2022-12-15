all:
	gcc -I SDL2/include -L SDL2/lib -o TicTacToe TicTacToe.c -lmingw32 -lSDL2main -lSDL2