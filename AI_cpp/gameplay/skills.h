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
    void init();
    void tick();
    void idle();
    void goToPoint(Point point);
    void fetchBall();
    void kick();
    void dribble();
    //function will get the robot to go to the ball
    //and aim position to aim at the goal
    void aim();

    //FUNCTIONS THAT USE UTILITIES
    void continueIdle();
    void continueGoToPoint();
    void continueKick();
    void continueFetchBall();
    void continueDribble();
    void continueAim();
    void stop();
  private:
    Point dest;
    Utilities utils;
    robotType type;
};

#endif /* SKILL_H_ */
