#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include "Ant.h"

#define W 800
#define H 600
#define NEST_SIZE 500

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Ants Simulation",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       W, H, 0);
    
    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Ant texture
    SDL_Surface* img = NULL;
    SDL_Texture* texture = NULL;
    img = IMG_Load("./utils/ant.png");
    texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    // Create nest
    std::vector<Ant> nest;
    nest.reserve(NEST_SIZE);
    for(int i=0; i<NEST_SIZE; i++){
        nest.push_back(Ant(renderer, texture));
    }

    int close = 0;
    srand(time(NULL));

    while (!close) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for(int i=0; i<NEST_SIZE; i++){
            nest[i].update(W, H);
            nest[i].show(renderer);
        }

        SDL_RenderPresent(renderer);

        // Events handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
            }
        }
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}