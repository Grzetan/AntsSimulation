#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Ant.h"

#define W 800
#define H 800
#define COLONY_POPULATION 10000
#define NEST_SIZE 30
#define PHERMONE_LIFE_SPAN 50

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
    int* world = new int[H*W];
    // 0 = nothing there
    // >0 = phermone and it's age
    // -1 = food
    // -2 = nest

    // Create nest
    for(int y=H/2-NEST_SIZE/2; y<H/2+NEST_SIZE/2; y++){
        for(int x=W/2-NEST_SIZE/2; x<W/2+NEST_SIZE/2; x++){
            world[y*H+x] = -2;
        }
    }

    int close = 0;
    srand(time(NULL));

    while (!close) {
        // Update and draw phermone
        for(int y=0; y<H; y++){
            for(int x=0; x<W; x++){
                int& tmp = world[y*H+x];
                if(tmp == 0){ // Empty space
                    setPixel(background, x, y, 255, 255, 255);
                }else if(tmp > 0){ // Phermones
                    tmp--;
                    int saturation = 255 - (int) ((float)tmp/PHERMONE_LIFE_SPAN*255);
                    setPixel(background, x, y, saturation, 255, saturation);
                }else if(tmp == -1){ // Food
                    setPixel(background, x, y, 0, 255, 0);
                }else if(tmp == -2){ // Nest
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

            if(nest[i].hasFood()){
                int idx = nest[i].getY()*H+nest[i].getX();
                if(world[idx] != -2){
                    world[idx] = PHERMONE_LIFE_SPAN;
                }
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

// // On linux compile with:
// // g++ -std=c++17 main.cpp -o prog -lSDL2 -ldl
// // On windows compile with (if using mingw)
// // g++ main.cpp -o prog.exe -lmingw32 -lSDL2main -lSDL2

// // C++ Standard Libraries
// #include <iostream>
// // Third Party
// #include <SDL2/SDL.h>

// void SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b){
    
//     SDL_LockSurface(surface);
//     uint8_t* pixelArray = (uint8_t*)surface->pixels;
//     pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+0] = g;
//     pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+1] = b;
//     pixelArray[y*surface->pitch +x*surface->format->BytesPerPixel+2] = r;
//     SDL_UnlockSurface(surface);
// }


// int main(int argc, char* argv[]){
//     // Create a window data type
//     // This pointer will point to the 
//     // window that is allocated from SDL_CreateWindow
//     SDL_Window* window=nullptr;

//     // Grab the window surface.
//     SDL_Surface* screen;

//     // Initialize the video subsystem.
//     // iF it returns less than 1, then an
//     // error code will be received.
//     if(SDL_Init(SDL_INIT_VIDEO) < 0){
//         std::cout << "SDL could not be initialized: " <<
//                   SDL_GetError();
//     }else{
//         std::cout << "SDL video system is ready to go\n";
//     }
//     // Request a window to be created for our platform
//     // The parameters are for the title, x and y position,
//     // and the width and height of the window.
//     window = SDL_CreateWindow("C++ SDL2 Window",
//             20,
//             20,
//             640,
//             480,
//             SDL_WINDOW_SHOWN);

//     screen = SDL_GetWindowSurface(window);


//     // Infinite loop for our application
//     bool gameIsRunning = true;
//     // Main application loop
//     while(gameIsRunning){
//         SDL_Event event;

//         int x,y;
//         Uint32 buttons;
//         buttons = SDL_GetMouseState(&x,&y);

//         // Start our event loop
//         while(SDL_PollEvent(&event)){
//             // Handle each specific event
//             if(event.type == SDL_QUIT){
//                 gameIsRunning= false;
//             }
//             if(event.button.button == SDL_BUTTON_LEFT){
//                 std::cout << "left mouse was pressed (" << x << "," << y << ")\n" ;
//                 SetPixel(screen, x, y, 255, 0, 0);
//             }
//             if(event.button.button == SDL_BUTTON_RIGHT){
//                 std::cout << "rightmouse was pressed (" << x << "," << y << ")\n" ;
//                 SetPixel(screen, x, y, 0, 0, 255);
//             }
//         }

//         SDL_UpdateWindowSurface(window);
//     }

//     // We destroy our window. We are passing in the pointer
//     // that points to the memory allocated by the 
//     // 'SDL_CreateWindow' function. Remember, this is
//     // a 'C-style' API, we don't have destructors.
//     SDL_DestroyWindow(window);
    
//     // our program.
//     SDL_Quit();
//     return 0;
// }