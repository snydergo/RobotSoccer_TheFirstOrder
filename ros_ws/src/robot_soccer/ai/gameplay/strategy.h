#include "../bookkeeping.h"
#include "plays.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{
public:

    Strategies()
    : robot1Plays(robotType::ally1)
    , robot2Plays(robotType::ally2)
    , strategy_st(StrategyState::idle)
    , startStrategy(StrategyState::goaliePower)
    {
    }

    void stop();
    void tick();

private:
    bool strategy_shouldRushGoal();
    Plays robot1Plays;
    Plays robot2Plays;

    enum class StrategyState {
        idle,
        start,
        goaliePower,
        gpGoalie,
        gpRush,
        rushSplitDefense,
        separateRushGoal,
        srgPlay
    } strategy_st, startStrategy;


};

#endif /* STRATEGY_H_ */
