#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{


public:
    Skills skill;
    Plays(){}

    Plays(robotType type)
    : type(type)
    {
    }

    void rushGoal();
    void playGoalie();
    void idle();
    void play_tick();

private:
    robotType type;
};

#endif // PLAYS_H
