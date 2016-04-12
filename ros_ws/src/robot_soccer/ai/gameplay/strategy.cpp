/*Contains statemachine for all of the different strategies that can be implimented. The Strategy
will change depending on the scores, opponent techniques and etc*/
#include "strategy.h"



void Strategies::stop()
{
    printf("stop strategy\n");
    strategy_st = StrategyState::idle;
}
void Strategies::start()
{
    printf("start strategy\n");
    strategy_st = StrategyState::rushSplitDefense;//startStrategy;
}

void Strategies::mark()
{
    printf("going to start Positions\n");
    strategy_st = StrategyState::mark;
}

void Strategies::tick()
{
    switch (strategy_st) {
    case StrategyState::idle:
//        std::cout << "Strategies::tick() idle"<< std::endl;
        robot1Plays.idle();
        robot2Plays.idle();
        break;

    case StrategyState::mark:
//        std::cout << "Strategies::tick() start"<< std::endl;
        robot1Plays.start(start1Location);
        robot2Plays.start(start2Location);
        break;

    case StrategyState::start:
        strategy_st = startStrategy;
	break;

    case StrategyState::separateRushGoal:
        std::cout << "Strategies:: tick() SeparateRushGoal" << std::endl;
        if (bkcalc::ballThreat()) {
            strategy_st = StrategyState::srgDefense;
            robot2Plays.rushGoal();
            robot1Plays.playGoalie();
            //robot2Plays.rushGoal();
        }else{
            strategy_st = StrategyState::srgOffense;
        }
        break;
    case StrategyState::srgDefense:
        std::cout << "Strategies::tick() SRGPlay" << std::endl;
        if (!bkcalc::ballThreat()) {
            strategy_st = StrategyState::srgOffense;
            robot2Plays.rushGoal();
            robot1Plays.playGoalie();
            //robot2Plays.rushGoal();
        }
        break;
    case StrategyState::srgOffense:
        if (bkcalc::ballThreat()) {
            strategy_st = StrategyState::srgDefense;
            robot2Plays.rushGoal();
            robot1Plays.playGoalie();
            //robot2Plays.rushGoal();
        }
        break;
    //##### GOALIE POWER STATES #####//
    case StrategyState::goaliePower:
        std::cout << "Strategies::tick() GoaliePower"<< std::endl;
        robot2Plays.rushGoal();
        if (bkcalc::ballThreat()) {
            std::cout << "Strategies::tick() playGoalie"<< std::endl;
            strategy_st = StrategyState::gpGoalie;
            robot1Plays.playGoalie();
        } else {
            std::cout << "Strategies::tick() rushingGoal"<< std::endl;
            strategy_st = StrategyState::gpRush;
            robot1Plays.rushGoal();
        }
        //robot2Plays.rushGoal();
		break;
    case StrategyState::gpGoalie:
//        std::cout << "Strategies::tick() GPGoalie (Playing Goalie)"<< std::endl;
        if (!bkcalc::ballThreat()) {
            strategy_st = StrategyState::gpRush;
            robot1Plays.rushGoal();
        }
        break;
    case StrategyState::gpRush:
//        std::cout << "Strategies::tick() GPRush (rushing Goal)"<< std::endl;
        if (bkcalc::ballThreat()) {
            strategy_st = StrategyState::gpGoalie;
            robot1Plays.playGoalie();
        }
        break;
    //RUSH SPLIT DEFENSE STATES
    case StrategyState::rushSplitDefense:
        if (bkcalc::ballThreat()) {
            robot2Plays.rushGoal();
            robot1Plays.playGoalie();
            strategy_st = StrategyState::rsdDefense;
        }else{
            robot2Plays.rushSplit(side::pos);
            robot1Plays.rushSplit(side::neg);
            strategy_st = StrategyState::rsdOffense;
        }
		break;
    case StrategyState::rsdOffense:
        if (bkcalc::ballThreat()) {
            robot2Plays.rushGoal();
            robot1Plays.playGoalie();
            strategy_st = StrategyState::rsdDefense;
        }
        break;
    case StrategyState::rsdDefense:
        if (!bkcalc::ballThreat()) {
            robot2Plays.rushSplit(side::pos);
            robot1Plays.rushSplit(side::neg);
            strategy_st = StrategyState::rsdOffense;
        }
        break;
    }

    //need to tick the play statemachine depending on change that occured
    robot1Plays.tick();
    robot2Plays.tick();
}


