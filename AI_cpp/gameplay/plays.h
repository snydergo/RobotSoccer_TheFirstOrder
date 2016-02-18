#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{
public:
    Skills skill = Skills();
    Plays();
    void rushGoal();
    void playGoalie();
    void idle();
    void play_tick();
};

#endif // PLAYS_H
