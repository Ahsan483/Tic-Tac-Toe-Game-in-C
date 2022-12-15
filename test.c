#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]){
    /* if SDL2 library could not be initalise, output error */
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Could not initialise SDL2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* create the console window */
    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

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


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    int width;
    int height;

    SDL_GetWindowSize(window, &width, &height);

    printf("width: %d; height: %d", width, height);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}