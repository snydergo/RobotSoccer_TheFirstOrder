#include "strategy.h"

enum class strategy {idle_st, start_st, GoaliePower_st,
                    RushSplitDefense_st} strategy_st;


void Strategies::init(){
    strategy_st = strategy::GoaliePower_st;
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
        robot1Plays.playGoalie();
        strategy_st = strategy::start_st;
		break;
    //state used to perform all necessary inits and start first strategy
    case strategy::start_st:
        robot1Plays.start();
        if(bkcalc::atLocation(robot1Plays.type, start1Location)){
            strategy_st = strategy::GoaliePower_st;
        }
        /*if(truegameStarted){
            strategy_st = strategy::GoaliePower_st;
        }*/
		break;
    case strategy::GoaliePower_st:
        if(field.currentStatus.ball.velocity.x < 0 &&
            field.currentStatus.ball.location.x < 0){
            robot1Plays.playGoalie();
        }else
            robot1Plays.rushGoal();
		break;
    case strategy::RushSplitDefense_st:
		break;
	}
}

void Strategies::stop()
{
	printf("stop strategy\n");
    strategy_st = strategy::idle_st;
}
