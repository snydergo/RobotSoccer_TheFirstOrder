#include "../bookkeeping.h"
#include "plays.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{
    public:
    Plays plays = Plays();
    Strategies();

    void strategy_stop();
    void strategy_tick();

};

#endif /* STRATEGY_H_ */
