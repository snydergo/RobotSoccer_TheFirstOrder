/*Contains statemachine for all of the different strategies that can be implimented. The Strategy
will change depending on the scores, opponent techniques and etc*/
#include "strategy.h"



void Strategies::stop()
{
    printf("stop strategy\n");
    strategy_st = StrategyState::idle;
}

void Strategies::tick()
{
    switch (strategy_st) {
    case StrategyState::idle:
    	//STATE USED TO WAIT UNTIL THE CAMERA DATA IS UPDATED
        std::cout << "Strategies::tick() idle"<< std::endl;
        //it is assumed robot is not in the right position
        if (fieldget::getRobotLoc(robot1Plays.allyNum).x != 0 &&
                fieldget::getRobotLoc(robot1Plays.allyNum).x != 0 &&
                fieldget::getRobot(robot1Plays.allyNum).theta != 0) {
            strategy_st = startStrategy;//strategy::start;
        }
        break;
    //state used to perform all necessary inits and start first strategy
    //has the robot go to a specified location on map when the game is going to start
    case StrategyState::start:
        std::cout << "Strategies::tick() start"<< std::endl;
        robot1Plays.start(start1Location);
        //robot2Plays.start(start2Location);
        if (calc::atLocation(fieldget::getRobotLoc(robot1Plays.allyNum), start1Location) /*&&
                bkcalc::atLocation(robot2Plays.allyNum, start2Location)*/) {
            std::cout << "at Start Location" << std::endl;
            strategy_st = startStrategy;
        }
		break;
    case StrategyState::separateRushGoal:
        std::cout << "Strategies:: tick() SeparateRushGoal" << std::endl;
        if (bkcalc::ballThreat()) {
            strategy_st = StrategyState::srgPlay;
            robot1Plays.playGoalie();
            //robot2Plays.rushGoal();
        }
        break;
    case StrategyState::srgPlay:
        std::cout << "Strategies::tick() SRGPlay" << std::endl;
        break;
    //##### GOALIE POWER STATES #####//
    case StrategyState::goaliePower:
        std::cout << "Strategies::tick() GoaliePower"<< std::endl;
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
        std::cout << "Strategies::tick() GPGoalie (Playing Goalie)"<< std::endl;
        if (!bkcalc::ballThreat()) {
            strategy_st = StrategyState::gpRush;
            robot1Plays.rushGoal();
        }
        break;
    case StrategyState::gpRush:
        std::cout << "Strategies::tick() GPRush (rushing Goal)"<< std::endl;
        if (bkcalc::ballThreat()) {
            strategy_st = StrategyState::gpGoalie;
            robot1Plays.playGoalie();
        }
        break;
    //RUSH SPLIT DEFENSE STATES
    case StrategyState::rushSplitDefense:
		break;
	}

    //need to tick the play statemachine depending on change that occured
    robot1Plays.tick();
    //robot2Plays.tick();
}


