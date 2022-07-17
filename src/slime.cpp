#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>

#define W 500
#define H 500
#define SPEED 2
#define NEST_SIZE 100
#define EVAPORATE_VALUE 2

void setPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b){
    SDL_LockSurface(surface);
    uint8_t* pixelArray = (uint8_t*)surface->pixels;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+0] = g;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+1] = b;
    pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+2] = r;
    SDL_UnlockSurface(surface);
}

SDL_Color getPixelColor(const SDL_Surface* pSurface, const int X, const int Y)
{
  // Bytes per pixel
  const Uint8 Bpp = pSurface->format->BytesPerPixel;

  /*
  Retrieve the address to a specific pixel
  pSurface->pixels  = an array containing the SDL_Surface' pixels
  pSurface->pitch       = the length of a row of pixels (in bytes)
  X and Y               = the offset on where on the image to retrieve the pixel; (0, 0) is the upper left corner
  */
  Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

  Uint32 PixelData = *(Uint32*)pPixel;

  SDL_Color Color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

  // Retrieve the RGB values of the specific pixel
  SDL_GetRGB(PixelData, pSurface->format, &Color.r, &Color.g, &Color.b);

  return Color;
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
            a.x += std::cos(a.angle) * SPEED;
            a.y += std::sin(a.angle) * SPEED;

            if(a.x < 0){
                a.x = 0;
                a.angle = M_PI - a.angle;
            }
            if(a.x > W){
                a.x = W - 1;
                a.angle = M_PI - a.angle;
            }
            if(a.y < 0){
                a.y = 0;
                a.angle = 2*M_PI - a.angle;
            }
            if(a.y > H){
                a.y = H - 1;
                a.angle = 2*M_PI - a.angle;
            }

            setPixel(background, a.x, a.y, 255,255,255);
        }

        // Evaporate trails
        for(int x=0; x<W; x++){
            for(int y=0; y<H; y++){
                SDL_Color currColor = getPixelColor(background, x, y);

                setPixel(background, x, y, std::max(currColor.r - EVAPORATE_VALUE, 0), 
                                           std::max(currColor.g - EVAPORATE_VALUE, 0), 
                                           std::max(currColor.b - EVAPORATE_VALUE, 0));
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
        }
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}