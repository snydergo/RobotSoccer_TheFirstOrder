#ifndef ROBOT_H
#define ROBOT_H

#include <string>

#include "fieldobject.h"

enum class robotType {ally1, ally2, enemy1, enemy2, none};

class Point;

class Robot: public FieldObject
{
public:
    double theta;
    //Constructors and other functions
    Robot(std::string tag, Point loc, Point vel, double theta, robotType type)
    : FieldObject(tag, loc, vel)
    , theta(theta)
 //   , plays(type)
    {
    }

    std::string toString();
};

#endif // ROBOT_H
