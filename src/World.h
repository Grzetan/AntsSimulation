#pragma once

enum Types{
    FOOD = -1,
    NEST = -2,
    EMPTY = 0,
    FOOD_PHERMONE = 1,
    HOME_PHERMONE = 2,
};

struct WorldPoint{
    Types type = Types::EMPTY;
    int lifeSpan = 0;
};