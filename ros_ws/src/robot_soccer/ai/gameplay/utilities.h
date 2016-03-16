#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "../bookkeeping.h"
#include "../mathfunctions.h"
#include "../dataclasses/robot.h"
#include "../types.h"
#include "../exceptionAI.h"

class Utilities
{
 public:
    Utilities(): type(robotType::ally1){}
    Utilities(robotType num)
    : type(num)
    {
    }


    void rotate(double angle);
    void init(Robot robot);
    void idle(Robot robot);
    void move(Robot robot, Point dir, double des_theta);
    void moveToPoint(Robot robot, Point point, double theta);
    void initKick(Robot robot);
    void uninitKick(Robot robot);
    void kick(Robot robot);
    void moveToCenter(Robot robot, double theta);
    void followBall(FieldObject ball, Robot robot, double theta);
    void dribble(Robot robot);
 private:
    robotType type;
};

#endif // UTILITIES_H
