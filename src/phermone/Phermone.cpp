#include "Phermone.h"

Phermone::Phermone(float x, float y, int lifeSpan){
    pos = {x, y};
    lifeSpan_ = lifeSpan;
    age_ = lifeSpan_;
}

SDL_Rect Phermone::Position::toSDLRect(){
    SDL_Rect pos;
    pos.x = (int) x;
    pos.y = (int) y;
    pos.w = 1;
    pos.h = 1;
    return pos;
}

void Phermone::update(){
    age_ -= 1;
}

void Phermone::show(SDL_Renderer* renderer){
    int saturation = 255 - (int) ((float)age_/lifeSpan_*255);
    SDL_SetRenderDrawColor(renderer, saturation, saturation, 255, 255);
    SDL_Rect r = pos.toSDLRect();
    SDL_RenderFillRect(renderer, &r);
}