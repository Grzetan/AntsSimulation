#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>

#define W 500
#define H 500
#define SPEED 2
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
    SDL_Window* win = SDL_CreateWindow("Slime Simulation",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       W,
                                       H,
                                       SDL_WINDOW_SHOWN);

    SDL_Surface* background = SDL_GetWindowSurface(win);

    int close = 0;

    srand(time(NULL));

    struct Agent{
        float x,y;
        float angle;
    };
    // Create agents

    std::vector<Agent> agents = {};
    agents.reserve(NEST_SIZE);
    for(int i=0; i<NEST_SIZE; i++){
        agents.push_back({W/2,H/2, (float)((float)rand() / (float)RAND_MAX * 2*M_PI)});
    }

    while (!close) {
        for(Agent& a : agents){
            a.x += std::sin(a.angle) * SPEED;
            a.y += std::cos(a.angle) * SPEED;

            if(a.x < 0 || a.x > W || a.y < 0 || a.y > H){
                if(a.x < 0){
                    a.x=0;
                }
                if(a.x > W){
                    a.x = W;
                }
                if(a.y < 0){
                    a.y = 0;
                }
                if(a.y > H){
                    a.y = H;
                }
                a.angle = (float)((float)rand() / (float)RAND_MAX * 2*M_PI);
            }

            setPixel(background, a.x, a.y, 255,255,255);
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
        }
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}