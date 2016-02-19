#include "../bookkeeping.h"
#include "plays.h"
#include "../types.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{

public:
    Plays robot1Plays;
    //Plays robot2Plays(robotType::ally2);
    void strategy_stop();
    void strategy_tick();

private:
    bool strategy_shouldRushGoal();

};

#endif /* STRATEGY_H_ */
