#include "Ant.h"

Ant::Ant(SDL_Renderer* renderer, const char* path, int x, int y, int w, int h, float speed){
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
    speed_ = speed;
    SDL_Surface* img = NULL;
    img = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
}

void Ant::show(SDL_Renderer* renderer){
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &pos);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}