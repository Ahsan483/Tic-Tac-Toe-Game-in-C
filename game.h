#ifndef GAME_H_
#define GAME_H_

#define BOARDCELLS 3
#define CELLWIDTH (screenWIDTH/ BOARDCELLS)
#define CELLHEIGHT (screenHEIGHT/ BOARDCELLS)

#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

/* different state of the game */
/* #define RUNNING_STATE 0
#define PLAYER_X_WON_STATE 1
#define PLAYER_O_WON_STATE 2
#define TIE_STATE 3 
#define QUIT_STATE 4 */

/* ########################### */
/* different state of the game */
/* RUNNING STATE = 0           */
/* PLAYER X WON STATE = 1      */
/* PLAYER O WON STATE = 2      */
/* TIE STATE = 3               */
/* QUIT STATE = 4              */
/* ########################### */
enum gameState {
    RUNNING_STATE,
    PLAYER_X_WON_STATE,
    PLAYER_O_WON_STATE,
    TIE_STATE,
    QUIT_STATE
};

typedef struct gameComponents {
    int board[BOARDCELLS * BOARDCELLS];
    int player; /* define the current player */
    int state; /* define the current state of the game */
} game_t;

#endif