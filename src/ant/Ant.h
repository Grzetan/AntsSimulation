#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <math.h>
#include "Phermone.h"

using std::vector;

class Ant{
    struct Position{
        float x, y, w, h;
        SDL_Rect toSDLRect();
    };
    Position pos;
    vector<Phermone> phermoneTrail;
    int phermoneLifeSpan_ = 50;
    float speed_, angle_ = 0; // Start by going left
    float TURN_ANGLE = 0.1; //Every turn is 0.1 radians

public:
    Ant(float=300, float=300, float=2, float=2, float=2);
    void show(SDL_Renderer*);
    void update(int, int);
    void move(int);
};