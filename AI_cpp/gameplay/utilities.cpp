#include "utilities.h"


//extern bool sendCmd_Rob1;
//extern bool sendCmd_Rob2;
//robot_soccer::controldata cmdRob1;
//robot_soccer::controldata cmdRob2;

void Utilities::rotate(double angle){
    printf("send command to move angle %f", angle);
}

void Utilities::move(Robot robot, Point dir, double des_theta){
    cmdRob1.cmdType = "mov";
    cmdRob1.x = robot.location.x;
    cmdRob1.y = robot.location.y;
    cmdRob1.theta = robot.theta;
    cmdRob1.x_cmd = dir.x;
    cmdRob1.y_cmd = dir.y;
    cmdRob1.theta_cmd = des_theta;
    sendCmd_Rob1 = true;
}

void Utilities::idle(){
    cmdRob1.cmdType = "idle";
}

void Utilities::dribble(){
    cmdRob1.cmdType = "dribble";
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

