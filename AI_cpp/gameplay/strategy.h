#include "../bookkeeping.h"
#include "plays.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{

public:
    Plays plays;

    void strategy_stop();
    void strategy_tick();

private:
    bool strategy_shouldRushGoal();

};

#endif /* STRATEGY_H_ */
