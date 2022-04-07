#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include "Ant.h"

#define W 800
#define H 600

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

    Ant ant1(renderer, "./utils/ant.png");
    Ant ant2(renderer, "./utils/ant.png");
    Ant ant3(renderer, "./utils/ant.png");
    Ant ant4(renderer, "./utils/ant.png");
    Ant ant5(renderer, "./utils/ant.png");

    int close = 0;
    srand(time(NULL));

    while (!close) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        ant1.update(W, H);
        ant1.show(renderer);
        ant2.update(W, H);
        ant2.show(renderer);
        ant3.update(W, H);
        ant3.show(renderer);
        ant4.update(W, H);
        ant4.show(renderer);
        ant5.update(W, H);
        ant5.show(renderer);

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