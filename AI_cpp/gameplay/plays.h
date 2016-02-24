#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{


public:
    Skills skill;
    Plays(){}

    Plays(robotType type)
    : allyNum(type)
    {
    }

    void rushGoal();
    void playGoalie();
    void idle();
    void tick();
    void init();
    void start();
    robotType allyNum;
};

#endif // PLAYS_H
