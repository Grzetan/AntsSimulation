#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

class Phermone{
    struct Position{
        float x, y;
        SDL_Rect toSDLRect();
    };
    Position pos;
    int lifeSpan_;
    int age_;
public:
    Phermone(float, float, int);
    void show(SDL_Renderer*);
    void update();

};