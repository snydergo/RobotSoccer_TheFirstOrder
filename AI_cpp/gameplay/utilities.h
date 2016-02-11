#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstddef>
#include <string>
#include "../mathfunctions.h"
#include "../dataclasses/point.h"
#include "../dataclasses/fieldobject.h"
#include "../dataclasses/robot.h"

class Robot;
class Utilities
{
    std::string name = "";
    robotType robotNum = none_rt;

   public:
    Utilities(std::string gvnName, robotType num);
    void rotate(double angle);
    void move(Robot* robot, Point* dir, double des_theta);
    void moveToPoint(Robot* robot, Point* point, double theta);
    void kick(double power, double theta);
    void moveToCenter(Robot* robot, double theta);
    void followBall(FieldObject* ball, Robot* robot, double theta);

};

#endif // UTILITIES_H
