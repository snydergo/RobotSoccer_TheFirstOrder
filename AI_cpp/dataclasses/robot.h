#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <cstddef>

#include "point.h"
#include "../gameplay/utilities.h"

//forward declaration to eliminate circular dependencies
class Utilities;
class Robot: public FieldObject{
public:
    double theta = 0;
    Utilities* utils = NULL;
    Robot(){}
    //Constructors and other functions
     Robot(std::string tag, Point loc, Point vel, double theta, robotType type);
        //: FieldObject(tag, loc, vel);
    std::string toString();
};

#endif // ROBOT_H