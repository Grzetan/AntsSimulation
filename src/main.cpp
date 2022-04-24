#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Ant.h"

#define W 500
#define H 500
#define NEST_SIZE 500
#define PHERMONE_LIFE_SPAN 50

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

    // Create nest
    std::vector<Ant> nest;
    nest.reserve(NEST_SIZE);
    for(int i=0; i<NEST_SIZE; i++){
        nest.push_back(Ant());
    }

    // World map
    int* world = new int[H*W];
    // 0 = nothing there
    // >0 phermone and it's age

    int close = 0;
    srand(time(NULL));

    while (!close) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for(int i=0; i<NEST_SIZE; i++){
            nest[i].update(W, H, world);
            nest[i].show(renderer);

            if(nest[i].hasFood()){
                world[nest[i].getY()*H+nest[i].getX()] = PHERMONE_LIFE_SPAN;
            }
        }

        SDL_Rect pos;
        for(int y=0; y<H; y++){
            for(int x=0; x<W; x++){
                if(world[y*H+x] > 0){
                    world[y*H+x]--;
                    int saturation = 255 - (int) ((float)world[y*H+x]/PHERMONE_LIFE_SPAN*255);
                    pos.x = x;
                    pos.y = y;
                    pos.w = 1;
                    pos.h = 1;
                    SDL_SetRenderDrawColor(renderer, saturation, saturation, 255, 255);
                    SDL_RenderFillRect(renderer, &pos);
                }
            }
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