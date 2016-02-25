#include "strategy.h"

enum class strategy {idle_st, start_st,
                     GoaliePower_st, GPGoalie_st, GPRush_st,
                    RushSplitDefense_st} strategy_st;

void Strategies::init(){
    strategy_st = strategy::idle_st;
    robot1Plays.init();
    robot2Plays.init();
}

bool strategy_shouldRushGoal()
{
	return false;
}

void Strategies::tick()
{
	switch (strategy_st)
	{
    case strategy::idle_st:
        std::cout << "Strategies::tick() idle_st"<< std::endl;
        //issue with the data not being initialized the first time
        //it is assumed robot is not in the right position
        if(fieldget::getRobotLoc(robot1Plays.allyNum).x != 0 &&
                fieldget::getRobotLoc(robot1Plays.allyNum).x != 0 &&
                fieldget::getRobot(robot1Plays.allyNum)->theta != 0){
            strategy_st = strategy::start_st;
        }
        break;
    //state used to perform all necessary inits and start first strategy
    case strategy::start_st:
        std::cout << "Strategies::tick() start_st"<< std::endl;
        robot1Plays.start();
        if(bkcalc::atLocation(robot1Plays.allyNum, start1Location)){
            std::cout << "at Start Location" << std::endl;
            strategy_st = strategy::GoaliePower_st;
        }
        /*if(truegameStarted){
            strategy_st = strategy::GoaliePower_st;
        }*/
		break;
    //GOALIE POWER STATES
    case strategy::GoaliePower_st:
        std::cout << "Strategies::tick() GoaliePower_st"<< std::endl;
        if(bkcalc::ballThreat()){
            std::cout << "Strategies::tick() playGoalie"<< std::endl;
            strategy_st = strategy::GPGoalie_st;
            robot1Plays.playGoalie();
        }else{
            std::cout << "Strategies::tick() rushingGoal"<< std::endl;
            strategy_st = strategy::GPRush_st;
            robot1Plays.rushGoal();
            //strategy_st = strategy::GPGoalie_st;
            //robot1Plays.playGoalie();
        }


		break;
    case strategy::GPGoalie_st:
        std::cout << "Strategies::tick() GPGoalie_st (Playing Goalie)"<< std::endl;
        if(!bkcalc::ballThreat()){
            strategy_st = strategy::GPRush_st;
            robot1Plays.rushGoal();
        }
        break;
    case strategy::GPRush_st:
        std::cout << "Strategies::tick() GPRush_st (rushing Goal)"<< std::endl;
        if(bkcalc::ballThreat()){
            strategy_st = strategy::GPGoalie_st;
            robot1Plays.playGoalie();
        }
        break;
    //RUSH SPLIT DEFENSE STATES
    case strategy::RushSplitDefense_st:
		break;
	}

    //need to tick the play statemachine depending on change that occured
    robot1Plays.tick();
    //robot2Plays.tick();
}

void Strategies::stop()
{
	printf("stop strategy\n");
    strategy_st = strategy::idle_st;
}
