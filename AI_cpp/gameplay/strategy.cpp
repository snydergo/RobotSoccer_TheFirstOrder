#include "strategy.h"

enum class strategy {idle_st, start_st, strategy_GoaliePower_st, 
					strategy_RushSplitDefense_st,} strategy_st;


bool strategy_shouldRushGoal()
{
	return false;
}

void Strategies::strategy_tick()
{
	switch (strategy_st)
	{
    case strategy::idle_st:
        plays.playGoalie();
        strategy_st = strategy::start_st;
		break;
    //state used to perform all necessary inits and start first strategy
    case strategy::start_st:
        strategy::strategy_GoaliePower_st;
		break;
    case strategy::strategy_GoaliePower_st:
        if(field.currentStatus.ball.velocity.x < 0 &&
            field.currentStatus.ball.location.x < 0){
            plays.playGoalie();
        }else
            plays.rushGoal();
		break;
    case strategy::strategy_RushSplitDefense_st:
		break;
	}
}

void Strategies::strategy_stop()
{
	printf("stop strategy\n");
    strategy_st = strategy::idle_st;
}
