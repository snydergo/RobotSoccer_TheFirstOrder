#ifndef SKILL_H_
#define SKILL_H_
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include "utilities.h"
#include "../types.h"

class Skills
{
public:
    Skills(RobotType type)
    : allyNum(type)
    , skill_st(SkillState::idle)
    , dest(allyGoal)
    , utils(type)
    {
    }

    void tick();
    void idle();
    void goToPoint(Point point, double dest_theta);
    void goToPoint(moveSpeed gvnspeed, Point point, double dest_theta);
    moveSpeed getSpeed();
    void fetchBall();
    void init_kick();
    void uninit_kick();
    void kick();
    void dribble();
    //function will get the robot to go to the ball
    //and aim position to aim at the goal
    void aim();



  private:

    //FUNCTIONS THAT USE UTILITIES
    void continueIdle();
    void continueGoToPoint();
    void initKicker();
    void uninitKicker();
    void continueKick();
    void continueFetchBall();
    void continueDribble();
    void continueAim();
    void stop();

    enum class SkillState {
        idle,
        gotoPoint,
        kick,
        initKick,
        uninitKick,
        fetchBall,
        dribble,
        aim
    } skill_st;


    Point dest;
    double theta_cmd;
    Utilities utils;
    RobotType allyNum;
    moveSpeed speed = moveSpeed::fast;
};

#endif /* SKILL_H_ */
