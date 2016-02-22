#ifndef SKILL_H_
#define SKILL_H_

#include "utilities.h"
#include "../types.h"

class Skills
{
public:
    Skills(robotType type)
        : type(type)
        , utils(type)
        , dest(0,0)
    {

    }

    Skills()
    : dest(0,0)
    {
    }

    void skill_tick();
    void skill_idle();
    void skill_goToPoint(Point* point);
    void skill_fetchBall();
    void skill_kick();
    void skill_dribble();
    //function will get the robot to go to the ball
    //and aim position to aim at the goal
    void skill_aim();

    //FUNCTIONS THAT USE UTILITIES
    void skill_continueIdle();
    void skill_continueGoToPoint();
    void skill_continueKick();
    void skill_continueFetchBall();
    void skill_continueDribble();
    void skill_continueAim();
    void skill_stop();
  private:
    Point dest;
    Utilities utils;
    robotType type;
};

#endif /* SKILL_H_ */
