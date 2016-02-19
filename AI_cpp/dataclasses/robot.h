#ifndef ROBOT_H
#define ROBOT_H

#include <string>

#include "fieldobject.h"
#include "../types.h"


class Robot: public FieldObject
{
public:
    double theta;
    //Constructors and other functions
    Robot(std::string tag, Point loc, Point vel, double theta, robotType type)
    : FieldObject(tag, loc, vel)
    , theta(theta)
    {
    }

    std::string toString();
};

#endif // ROBOT_H
