#include "Ant.h"

Ant::Ant(float x, float y, int w, int h, float speed){
    pos = {x,y,w,h};
    angle_ = (float)rand() / (float)RAND_MAX * 2*M_PI;
    speed_ = speed;
}

int Ant::findDirection(int W, int H, WorldPoint* world){
    // Look for phermone
    int dir=1, max=0;
    Types type;
    int sensorCount[] = {0, 0, 0}; // Phermone amount in each spot
    float angles[] = {-1, 0, 1}; // Sensors angles
    float x, y, Xmin, Xmax, Ymin, Ymax;

    for(int i=0; i<3; i++){
        x = pos.x + sensorDistance * cos(angle_ + sensorAngle * angles[i]);
        y = pos.y + sensorDistance * sin(angle_ + sensorAngle * angles[i]);
        Ymin = std::max(std::min((int) (y-sensorArea/2), H-1), 0);
        Ymax = std::max(std::min((int) (y+sensorArea/2), H-1), 0);
        Xmin = std::max(std::min((int) (x-sensorArea/2), W-1), 0);
        Xmax = std::max(std::min((int) (x+sensorArea/2), W-1), 0);
        for(int y_=Ymin; y_ < Ymax; y_++){
            for(int x_=Xmin; x_ < Xmax; x_++){
                type = world[y_*H+x_].type;
                if(!hasFood_ && type == Types::FOOD){
                    return angles[i];
                }else if(hasFood_ && type == Types::NEST){
                    return angles[i];
                }else if((type == Types::FOOD_PHERMONE && !hasFood_)){
                    sensorCount[i] += world[y_*H+x_].lifeSpan;
                }
            }
        }
        if(sensorCount[i] > max){
            if(hasFood_){
                std::cout << dir << std::endl;
            }
            max = sensorCount[i];
            dir = angles[i];
        }
    }
    if(max == 0){
        return -2; // Move randomly
    }

    return angles[dir];
}

void Ant::update(int W, int H, WorldPoint* world){
    // Get best direction
    int dir = findDirection(W, H, world);
    if(dir != -2){
        angle_ += sensorAngle*dir;
    }else{
        float theta = (float)rand() / (float)RAND_MAX;

        // Randomly turn left or right or keep moving forward
        if(theta < 0.3333){
            angle_ += TURN_ANGLE;
        }else if(theta >= 0.3333 && theta < 0.6666){
            angle_ -= TURN_ANGLE;
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

    // Pick up food
    int idx = (int) pos.y*H + (int) pos.x;
    Types& type = world[idx].type;
    if(type == Types::FOOD && !hasFood_){
        type = Types::EMPTY;
        hasFood_ = true;
        angle_ += M_PI; // Turn 180 degrees
    }

    // Leave food in nest
    if(type == Types::NEST && hasFood_){
        hasFood_ = false;
        angle_ += M_PI; // Turn 180 degrees
    }

    // Keep the angle between 0 and 2PI radians
    if(angle_ >= 2*M_PI || angle_ < 0){
        angle_ = 0;
    }
}

bool Ant::hasFood(){
    return hasFood_;
}

int Ant::getX(){
    return pos.x;
}

int Ant::getY(){
    return pos.y;
}