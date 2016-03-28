/* Class that inherits from Field Object except it contains a theta and the 
tags are used for knowing which robot to send commands to if they are an ally*/
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
    Robot(robotType tag, Point loc, Point vel, double theta)
    : FieldObject(tag, loc, vel)
    , theta(theta)
    {
    }

    Robot()
    : FieldObject(robotType::none, Point(), Point())
    , theta(0)
    {
    }

    std::string toString();
};

#endif // ROBOT_H
