#include "strategy.h"

enum class strategy {idle_st, start_st, GoaliePower_st,
                    RushSplitDefense_st} strategy_st;

int count = 0;
void Strategies::init(){
    strategy_st = strategy::idle_st;
    robot1Plays.init();
    robot2Plays.init();
    roleSwitch = true;
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
        if(count++ == 50){
            strategy_st = strategy::start_st;
            roleSwitch = true;
        }
		break;
    //state used to perform all necessary inits and start first strategy
    case strategy::start_st:
        std::cout << "Strategies::tick() start_st"<< std::endl;
        robot1Plays.start();
        if(bkcalc::atLocation(robot1Plays.allyNum, start1Location)){
            std::cout << "at Start Location" << std::endl;
            strategy_st = strategy::GoaliePower_st;
            roleSwitch = true;
        }
        /*if(truegameStarted){
            strategy_st = strategy::GoaliePower_st;
        }*/
		break;
    case strategy::GoaliePower_st:
        std::cout << "Strategies::tick() GoaliePower_st"<< std::endl;
        if(field.currentStatus.ball.velocity.x < 0 &&
            field.currentStatus.ball.location.x < 0){
            std::cout << "Strategies::tick() playingGoalie"<< std::endl;
            robot1Plays.playGoalie(roleSwitch);
        }else
            //std::cout << "Strategies::tick() rushingGoal"<< std::endl;
            //robot1Plays.rushGoal(roleSwitch);
            robot1Plays.playGoalie(roleSwitch);
		break;
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
