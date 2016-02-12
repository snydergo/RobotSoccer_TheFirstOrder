#include "utilities.h"

#include "../bookkeeping.h"

void Utilities::rotate(double angle){
    printf("send command to move angle %f", angle);
}

void Utilities::move(Robot robot, Point dir, double des_theta){
    /*robot_soccer::controldata msg = new robot_soccer::controldata();
    msg.cmdType = mov
    msg.x_dir = dir->x;
    msg.y_dir = dir->y;
    msg.cur_theta = robot->theta;
    msg.des_theta = des_theta;*/
    //send info to Matt
}

void Utilities::moveToPoint(Robot robot, Point point, double theta){
    Point dir = calc::directionToPoint(robot.location, point);
    move(robot, dir, theta);
}

void Utilities::kick(double power, double theta){
    printf("ball kicked with power %f and angle %f\n", power, theta);
    //msg.cmdType = kick
}

void Utilities::moveToCenter(Robot robot, double theta){
    Point movVector = calc::directionToPoint(robot.location, center);
    move(robot, movVector, theta);
}

void Utilities::followBall(FieldObject ball, Robot robot, double theta){
    printf("following ball\n");
    Point movVector = calc::directionToPoint(robot.location, ball.location);
    move(robot, movVector, theta);
}

