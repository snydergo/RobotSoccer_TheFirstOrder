#include "../bookkeeping.h"
#include "plays.h"

#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategies{
public:

    Strategies()
    : robot1Plays(RobotType::ally1)
    , robot2Plays(RobotType::ally2)
    , strategy_st(StrategyState::idle)
    , startStrategy(StrategyState::rushSplitDefense)
    {
    }
    void mark();
    void start();
    void stop();
    void tick();

private:
    bool strategy_shouldRushGoal();
    Plays robot1Plays;
    Plays robot2Plays;

    enum class StrategyState {
        idle,
        mark,
        start,
        goaliePower,
        gpGoalie,
        gpRush,
        rushSplitDefense,
        rsdDefense,
        rsdOffense,
        separateRushGoal,
        srgOffense,
        srgDefense
    } strategy_st, startStrategy;


};

#endif /* STRATEGY_H_ */
