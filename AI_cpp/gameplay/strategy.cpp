#include "strategy.h"

enum strategy_st {strategy_idle_st, strategy_start_st, strategy_GoaliePower_st, 
					strategy_RushSplitDefense_st,} strategy_st;


bool strategy_shouldRushGoal()
{
	return false;
}

void strategy_tick()
{
	switch (strategy_st)
	{
	case strategy_idle_st:
		break;
	case strategy_start_st:
		break;
	case strategy_GoaliePower_st:
		break;
	case strategy_RushSplitDefense_st:
		break;
	}
}

void strategy_stop()
{
	printf("stop strategy\n");
    strategy_st = strategy_idle_st;
}
