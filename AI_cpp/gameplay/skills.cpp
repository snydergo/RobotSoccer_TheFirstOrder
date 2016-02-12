#include "skills.h"


enum skill_states {skill_none, skill_goingToPoint, skillState_circleBall,
	skillState_fetchBall, skillState_dribbleBall} skill_state;

void skill_goToPoint(Point* point)
{
	skill_state = skill_goingToPoint;
}

void skill_continueGoToPoint(Robot* robot, Point* point)
{

}

void skill_continueCircularBallMotion(float tangentialVelocity, float perpendicularVelocity, bool turnToBounce)
{

}

// dir = +1 (CW) or -1 (CCW).
void skill_circleBall(int dir)
{
	printf("start circle ball\n");
	skill_state = skillState_circleBall;
}

// dir = +1 (CW) or -1 (CCW).
void skill_fetchBall()
{
	printf("start fetch ball\n");
	skill_state = skillState_fetchBall;
}

void skill_dribbleBall()
{
	printf("start dribble ball\n");
	skill_state = skillState_dribbleBall;
}

void skill_continueCircleBall()
{

}

void skill_continueFetchBall()
{

}

void skill_continueDribbleBall()
{

}

void skill_stop()
{

}

void skill_tick()
{
	switch (skill_state)
	{
	case skill_goingToPoint:
        //skill_continueGoToPoint(robot1currentPosition, pointToGoTo);
		break;
	case skillState_circleBall:
		skill_continueCircleBall();
		break;
	case skillState_fetchBall:
		skill_continueFetchBall();
		break;
	case skillState_dribbleBall:
		skill_continueDribbleBall();
		break;
	}
}
