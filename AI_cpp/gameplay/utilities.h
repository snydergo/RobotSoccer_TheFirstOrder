#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "../bookkeeping.h"
#include "../mathfunctions.h"
#include "../dataclasses/robot.h"
#include "../types.h"


class Utilities
{
 public:
    Utilities(): type(robotType::ally1){}
    Utilities(robotType num)
    : type(num)
    {
    }


    void rotate(double angle);
    void idle();
    void move(Robot robot, Point dir, double des_theta);
    void moveToPoint(Robot robot, Point point, double theta);
    void kick(double power, double theta);
    void moveToCenter(Robot robot, double theta);
    void followBall(FieldObject ball, Robot robot, double theta);
    void dribble();
 private:
    robotType type;
};

#endif // UTILITIES_H
