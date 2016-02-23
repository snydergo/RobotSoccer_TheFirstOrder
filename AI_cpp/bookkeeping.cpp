#include "bookkeeping.h"

bool pend(sem semaphore) {
    if(semaphore){
        semaphore--;
        return true;
    }else
        return false;
}

void post(sem semaphore) {
    semaphore++;
}

Point center = Point(0,0);
Point enemyGoal = Point(GOAL_XLOCATION,0);
Point allyGoal = Point(-GOAL_XLOCATION,0);
FieldCoord field;
bool visionUpdated;
sem visionMsg_sem = 1;
sem ctrlMsg_sem = 1;
GameStatus visionStatus_msg;

bool sendCmd_Rob1 = true;
bool sendCmd_Rob2 = false;
robot_soccer::controldata cmdRob1;
robot_soccer::controldata cmdRob2;
