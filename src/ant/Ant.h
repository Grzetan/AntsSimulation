#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <math.h>

using std::vector;

class Ant{
    struct Position{
        float x, y, w, h;
    };
    Position pos;
    float speed_, angle_;
    float TURN_ANGLE = 0.1; //Every turn is 0.1 radians
    float sensorDistance = 5, sensorAngle = M_PI / 3, sensorArea = 4;

public:
    Ant(float=300, float=300, float=3, float=3, float=2);
    void update(int, int, int*);
    bool hasFood();
    int getX();
    int getY();
};