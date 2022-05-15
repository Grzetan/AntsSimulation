#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Ant.h"
#include "World.h"

#define W 500
#define H 500
#define COLONY_POPULATION 100
#define NEST_SIZE 30
#define PHERMONE_LIFE_SPAN 300

void setPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b){
    SDL_LockSurface(surface);
    uint8_t* pixelArray = (uint8_t*)surface->pixels;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+0] = g;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+1] = b;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+2] = r;
    SDL_UnlockSurface(surface);
}

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Ants Simulation",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       W,
                                       H,
                                       SDL_WINDOW_SHOWN);

    SDL_Surface* background = SDL_GetWindowSurface(win);

    // Create nest
    std::vector<Ant> nest;
    nest.reserve(COLONY_POPULATION);
    for(int i=0; i<COLONY_POPULATION; i++){
        nest.push_back(Ant(W/2, H/2));
    }

    // World map
    WorldPoint* world = new WorldPoint[H*W];

    // Create nest
    for(int y=H/2-NEST_SIZE/2; y<H/2+NEST_SIZE/2; y++){
        for(int x=W/2-NEST_SIZE/2; x<W/2+NEST_SIZE/2; x++){
            world[y*H+x].type = Types::NEST;
        }
    }

    // Create food
    // for(int y=0; y<50; y++){
    //     for(int x=0; x<50; x++){
    //         world[y*H+x].type = Types::FOOD;
    //     }
    // }

    int close = 0;
    int x,y; // For mouse coordinates
    srand(time(NULL));

    while (!close) {
        // Update and draw phermone
        for(int y=0; y<H; y++){
            for(int x=0; x<W; x++){
                Types& type = world[y*H+x].type;
                int& lifeSpan = world[y*H+x].lifeSpan;
                if(type == Types::EMPTY){ // Empty space
                    setPixel(background, x, y, 255, 255, 255);
                }else if(type == Types::FOOD_PHERMONE){ // Phermones
                    int saturation = 255 - (int) ((float)lifeSpan/PHERMONE_LIFE_SPAN*255);
                    setPixel(background, x, y, 128, 128, saturation);
                    lifeSpan--;
                    if(lifeSpan == 0){
                        type = Types::EMPTY;
                    }
                }else if(type == Types::HOME_PHERMONE){ // Phermones
                    int saturation = 255 - (int) ((float)lifeSpan/PHERMONE_LIFE_SPAN*255);
                    setPixel(background, x, y, saturation, 255, saturation);
                    lifeSpan--;
                    if(lifeSpan == 0){
                        type = Types::EMPTY;
                    }
                }else if(type == Types::FOOD){ // Food
                    setPixel(background, x, y, 0, 0, 255);
                }else if(type == Types::NEST){ // Nest
                    setPixel(background, x, y, 0, 0, 0);
                }
            }
        }

        // Update and draw ants
        for(int i=0; i<COLONY_POPULATION; i++){
            nest[i].update(W, H, world);
            int x = nest[i].getX(), y = nest[i].getY();
            for(int x_=x; x_<x+2; x_++){
                for(int y_=y; y_<y+2; y_++){
                    setPixel(background, x_, y_, 255, 0, 0);
                }
            }

            int idx = nest[i].getY()*H+nest[i].getX();
            if(world[idx].type == Types::EMPTY || world[idx].type == Types::FOOD_PHERMONE){
                if(nest[i].hasFood()){
                    world[idx].type = Types::FOOD_PHERMONE;
                }
                // else{
                //     world[idx].type = Types::HOME_PHERMONE;
                // }
                world[idx].lifeSpan = PHERMONE_LIFE_SPAN;
            }
        }

        SDL_UpdateWindowSurface(win);

        // Events handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
            }
            SDL_GetMouseState(&x,&y);
            if(event.button.button == SDL_BUTTON_LEFT){
                for(int i=x-5; i<x+5; i++){
                    for(int j=y-5; j<y+5; j++){
                        world[j*H+i].type = Types::FOOD_PHERMONE;
                        world[j*H+i].lifeSpan = PHERMONE_LIFE_SPAN;
                    }
                }
            }
            if(event.button.button == SDL_BUTTON_RIGHT){
                // SetPixel(screen, x, y, 0, 0, 255);
            }
        }
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}