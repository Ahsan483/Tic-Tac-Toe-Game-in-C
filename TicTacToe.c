#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "game.h"

/* #################################################### */
/* initialised screen size as global variables          */
/* uses extern in order for game.h to use               */
/* screen sizes might change due to re-sizing of screen */
/* #################################################### */
float screenWIDTH = 800.0;
float screenHEIGHT = 600.0;
int menu = 0;

void render_game(SDL_Renderer *renderer, const game_t *game);
void render_running_state(SDL_Renderer *renderer, const game_t *game);
void render_game_over_state(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color);
void click_on_cell(game_t *game, int row, int column);

int main(int argc, char *argv[]){


    /* if SDL2 library could not be initalise, output error */
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Could not initialise SDL2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (menu == 0)
    {

    }
    /* create the console window */
    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWIDTH, screenHEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    
    /* if window console cannot be created, output error */
    if (window == NULL){
        printf("Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* rendering of the window */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    game_t game = {
         .board = { PLAYER_O, EMPTY, EMPTY,
                    EMPTY, PLAYER_X, EMPTY,
                    EMPTY, EMPTY, PLAYER_X},
        .player = PLAYER_X,
        .state = TIE_STATE
    };

    SDL_Event event; /* handle the current processing event */
    while (game.state != QUIT_STATE){
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.state = QUIT_STATE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    click_on_cell(&game, event.button.y/CELLHEIGHT, event.button.x/CELLWIDTH);
                    break;

                default: {}; /* to handle the next frame */
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer); /* make the rendering visible due to double buffering */
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255};
const SDL_Color PLAYER_X_COLOR = { .r = 255, .g = 50, .b = 50};
const SDL_Color PLAYER_O_COLOR = { .r = 50, .g = 100, .b = 255 };
const SDL_Color TIE_COLOR = { .r = 100, .g = 100, .b = 100 };

void render_grid(SDL_Renderer *renderer, const SDL_Color *color){
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    for (int i = 1; i < BOARDCELLS; ++i){
        /* render the horizontal lines */
        SDL_RenderDrawLine(renderer, i*CELLWIDTH, 0, i*CELLWIDTH, screenHEIGHT);

        /* render the vertical lines */
        SDL_RenderDrawLine(renderer, 0, i*CELLHEIGHT, screenWIDTH, i*CELLHEIGHT);
    }
}

void render_x(SDL_Renderer *renderer, int row, int column, const SDL_Color *color){
    const float half_box_side = fmin(CELLWIDTH, CELLHEIGHT) * 0.25;
    const float center_x = CELLWIDTH * 0.5 + column * CELLWIDTH;
    const float center_y = CELLHEIGHT * 0.5 + row * CELLHEIGHT;

    thickLineRGBA(renderer, center_x-half_box_side, center_y-half_box_side, center_x+half_box_side, center_y+half_box_side, 10, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x+half_box_side, center_y-half_box_side, center_x-half_box_side, center_y+half_box_side, 10, color->r, color->g, color->b, 255);
}

void render_o(SDL_Renderer *renderer, int row, int column, const SDL_Color *color){
    const float half_box_side = fmin(CELLWIDTH, CELLHEIGHT) * 0.25;
    const float center_x = CELLWIDTH * 0.5 + column * CELLWIDTH;
    const float center_y = CELLHEIGHT * 0.5 + row * CELLHEIGHT;

    filledCircleRGBA(renderer, center_x, center_y, half_box_side + 5, color->r, color->g, color->b, 255);
    filledCircleRGBA(renderer, center_x, center_y, half_box_side - 5, 0, 0, 0, 255);
    
}

void render_board(SDL_Renderer *renderer, const int *board, const SDL_Color *player_x_color, const SDL_Color *player_o_color){
    for (int i = 0; i < BOARDCELLS; ++i){
        for (int j = 0; j < BOARDCELLS; ++j){
            switch (board[i * BOARDCELLS +j]){
                case PLAYER_X:
                    render_x(renderer, i, j, player_x_color);
                    break;

                case PLAYER_O:
                    render_o(renderer, i, j, player_o_color);
                    break;

                default: {};
            }
        }
    }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game){
    render_grid(renderer, &GRID_COLOR);
    render_board(renderer, game->board, &PLAYER_X_COLOR, &PLAYER_O_COLOR);
}

void render_game_over_state(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color){
    render_grid(renderer, color);
    render_board(renderer, game->board, color, color);
}

/* Render the game in the screen */
void render_game(SDL_Renderer *renderer, const game_t *game){
    switch (game -> state) {
        case RUNNING_STATE:
            render_running_state(renderer, game);
            break;
        
        case PLAYER_X_WON_STATE:
            render_game_over_state(renderer, game, &PLAYER_X_COLOR);
            break;

        case PLAYER_O_WON_STATE:
            render_game_over_state(renderer, game, &PLAYER_O_COLOR);
            break;

        case TIE_STATE:
            render_game_over_state(renderer, game, &TIE_COLOR);
            break;

        default: {};    
    }
}

// switch player turn
void switch_player(game_t *game){
    if (game->player == PLAYER_X){
        game->player = PLAYER_O;
    }
    else{
        game->player = PLAYER_X;
    }
}

int check_player_won(game_t *game, int player){
    int row_count = 0;
    int col_count = 0;
    int diag1_count = 0;
    int diag2_count = 0;

    // check each row
    for (int row = 0; row < BOARDCELLS; row++){
        // check each column
        for (int col = 0; col < BOARDCELLS; col++){
            if (game->board[row * BOARDCELLS + col] == player){
                row_count++;
            }

            if (game->board[col * BOARDCELLS + row] == player){
                col_count++;
            }
        }
        if (row_count >= BOARDCELLS || col_count >= BOARDCELLS){
            return 1;
        }

        row_count = 0;
        col_count = 0;

        if (game->board[row * BOARDCELLS + row] == player){
            diag1_count++;
        }

        if (game->board[row * BOARDCELLS + BOARDCELLS - row - 1] == player){
            diag2_count++;
        }
    }

    return diag1_count >= BOARDCELLS || diag2_count >= BOARDCELLS;
}

int count_cells(const int *board, int cell){
    int count = 0;
    for (int i = 0; i < BOARDCELLS * BOARDCELLS; i++){
        if (board[i] == cell){
            count++;
        }
    }

    return count;
}

void game_over_condition(game_t *game){
    if (check_player_won(game, PLAYER_X)){
        game->state = PLAYER_X_WON_STATE;
    }
    else if (check_player_won(game, PLAYER_O)){
        game->state = PLAYER_O_WON_STATE;
    }
    else if (count_cells(game->board, EMPTY) == 0){
        game->state = TIE_STATE;
    }

}

void player_turn(game_t *game, int row, int column){
    if (game->board[row * BOARDCELLS + column] == EMPTY){
        game->board[row * BOARDCELLS + column] = game->player;
        switch_player(game);
        game_over_condition(game);
    }
}

void reset_game(game_t *game){
    game->player = PLAYER_X;
    game->state = RUNNING_STATE;
    for (int i = 0; i < BOARDCELLS * BOARDCELLS; i++){
        game->board[i] = EMPTY;
    }
}

/* Check which cells in the game is clicked on */
void click_on_cell(game_t *game, int row, int column){
    if (game->state == RUNNING_STATE){
        player_turn(game, row, column);
    }
    else{
        reset_game(game);
    }
}