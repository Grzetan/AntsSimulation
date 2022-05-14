#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include "World.h"

class Ant{
    struct Position{
        float x, y;
        int w, h;
    };
    Position pos;
    float speed_, angle_;
    float TURN_ANGLE = 0.1; //Every turn is 0.1 radians
    float sensorDistance = 15, sensorAngle = M_PI / 3, sensorArea = 4;
    bool hasFood_ = false;
    int findDirection(int, int, WorldPoint*);

public:
    Ant(float=300, float=300, int=3, int=3, float=2);
    void update(int, int, WorldPoint*);
    bool hasFood();
    int getX();
    int getY();
};