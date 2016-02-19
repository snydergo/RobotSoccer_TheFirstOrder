#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "../bookkeeping.h"
#include "../mathfunctions.h"
#include "../dataclasses/robot.h"


class Utilities
{
    std::string name;
    robotType robotNum;

   public:
    Utilities(): robotNum(robotType::ally1),name("ally1"){}
    Utilities(std::string gvnName, robotType num)
    : name(gvnName)
    , robotNum(num)
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
};

#endif // UTILITIES_H
