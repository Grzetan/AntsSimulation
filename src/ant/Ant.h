#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Ant{
    SDL_Rect pos;
    SDL_Texture* texture;
    float vel_, angle_, speed_;

public:
    Ant(SDL_Renderer*, const char*, int=0, int=0, int=20, int=20, float=2);
    void show(SDL_Renderer*);
    void update();
};