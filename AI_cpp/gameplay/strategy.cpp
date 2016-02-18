#include "strategy.h"

enum class strategy {idle_st, start_st, strategy_GoaliePower_st, 
					strategy_RushSplitDefense_st,} strategy_st;


bool strategy_shouldRushGoal()
{
	return false;
}

void strategy_tick()
{
	switch (strategy_st)
	{
    case strategy::idle_st:
		break;
    case strategy::start_st:
		break;
    case strategy::strategy_GoaliePower_st:
		break;
    case strategy::strategy_RushSplitDefense_st:
		break;
	}
}

void strategy_stop()
{
	printf("stop strategy\n");
    strategy_st = strategy::idle_st;
}
