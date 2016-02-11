#include "bookkeeping.h"


GameStatus initGameStatus(){
    Point point(0,0);
    Robot ally1("ally1", point, point, 0, ally1_rt);
    Robot ally2("ally2", point, point, 0, ally2_rt);
    Robot enemy1("enemy1", point, point, 0, enemy1_rt);
    Robot enemy2("enemy2", point, point, 0, enemy2_rt);
    FieldObject ball("ball", point, point);
    return GameStatus(ally1, ally2, enemy1, enemy2, ball);
}

bool pend(sem semaphore){
    if(semaphore){
        semaphore--;
        return true;
    }else
        return false;
}

void post(sem semaphore){
    semaphore++;
}

Point center = Point(0,0);

FieldCoord field;
bool visionUpdated;
sem visionMsg_sem = 1;
sem ctrlMsg_sem = 1;
GameStatus visionStatus_msg;

