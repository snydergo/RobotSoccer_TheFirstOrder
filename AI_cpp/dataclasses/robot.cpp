#include "robot.h"

//Robot Object functions
Robot::Robot(std::string tag, Point* loc, Point* vel, double theta, robotType type)
    : FieldObject(tag, loc, vel){
    this->theta = theta;
    this->utils = new Utilities(tag, type);
}

std::string Robot::toString(){
    return "null";
}
