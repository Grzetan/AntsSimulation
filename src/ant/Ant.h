#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

class Ant{
    struct Position{
        float x, y, w, h;
        SDL_Rect toSDLRect();
    };
    Position pos;
    SDL_Texture* texture;
    float speed_, angle_ = 0; // Start by going left
    float TURN_ANGLE = 0.05; //Every turn is 0.2 radians

public:
    Ant(SDL_Renderer*, const char*, int=300, int=300, int=50, int=50, float=2);
    void show(SDL_Renderer*);
    void update(int, int);
    void move(int);
};