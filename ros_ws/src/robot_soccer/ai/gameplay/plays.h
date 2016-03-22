#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{


public:
    Skills skill;
    Plays(){}

    Plays(robotType type)
    : allyNum(type),
      skill(type)
    {
    }

    void initKicker();
    void uninitKicker();
    void rushGoal();
    void playGoalie();
    void idle();
    void tick();
    void init();
    void start(Point startLoc);
    robotType allyNum;
};

#endif // PLAYS_H
