#include "Ant.h"

Ant::Ant(float x, float y, float w, float h, float speed){
    pos = {x,y,w,h};
    angle_ = (float)rand() / (float)RAND_MAX * 2*M_PI;
    speed_ = speed;
}

SDL_Rect Ant::Position::toSDLRect(){
    SDL_Rect pos;
    pos.x = (int) x;
    pos.y = (int) y;
    pos.w = (int) w;
    pos.h = (int) h;
    return pos;
}

void Ant::move(int dir){
    angle_ += dir * TURN_ANGLE;
}

void Ant::update(int W, int H, int* phermones){
    // Look for phermone
    int dir = 0, max=0;
    int phermoneCount[] = {0,0,0}; // Phermone amount in each spot
    float angles[] = {-1, 0, 1}; // Sensors angles
    float x, y;
    for(int i=0; i<3; i++){
        x = pos.x + sensorDistance * cos(angle_ + sensorAngle * angles[i]);
        y = pos.y + sensorDistance * sin(angle_ + sensorAngle * angles[i]);
        for(int y_=y-sensorArea/2; y_ < y+sensorArea/2; y_++){
            for(int x_=x-sensorArea/2; x_ < x+sensorArea/2; x_++){
                if(phermones[y_*H+x_] > 0){
                    phermoneCount[i] += phermones[y_*H+x_];
                }
            }
        }
        if(phermoneCount[i] > max){
            max = phermoneCount[i];
            dir = angles[i];
        }
    }

    if(max != 0){
        angle_ += TURN_ANGLE*dir;
    }else{
        float theta = (float)rand() / (float)RAND_MAX;

        // Randomly turn left or right or keep moving forward
        if(theta < 0.3333){
            angle_ += TURN_ANGLE;
        }else if(theta >= 0.3333 && theta < 0.6666){
            angle_ -= TURN_ANGLE;
        }

        // Keep the angle between 0 and 2PI radians
        if(angle_ >= 2*M_PI || angle_ < 0){
            angle_ = 0;
        }
    }

    // Update position coordinates
    pos.x = pos.x + speed_ * cos(angle_);
    pos.y = pos.y + speed_ * sin(angle_);

    // Check for collision
    if(pos.x < 0){
        pos.x = 0;
        angle_ = M_PI - angle_;
    }else if(pos.x + pos.w > W){
        pos.x = W - pos.w;
        angle_ = M_PI - angle_;
    }
    if(pos.y < 0){
        pos.y = 0;
        angle_ = 2*M_PI - angle_;
    }else if(pos.y + pos.h > H){
        pos.y = H - pos.h;
        angle_ = 2*M_PI - angle_;
    }
}

void Ant::show(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect r = pos.toSDLRect();
    SDL_RenderFillRect(renderer, &r);
}

bool Ant::hasFood(){
    return true; // TODO
}

int Ant::getX(){
    return (int) pos.x;
}

int Ant::getY(){
    return (int) pos.y;
}