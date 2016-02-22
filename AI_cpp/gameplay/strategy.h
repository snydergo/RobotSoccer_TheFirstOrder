#include "../bookkeeping.h"
#include "plays.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{

public:
    Strategies()
    : robot1Plays(robotType::ally1)
    , robot2Plays(robotType::ally2)
    {
    }


    void strategy_stop();
    void strategy_tick();

private:
    bool strategy_shouldRushGoal();

    Plays robot1Plays;
    Plays robot2Plays;

};

#endif /* STRATEGY_H_ */
