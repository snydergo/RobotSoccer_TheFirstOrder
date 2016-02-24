#include "utilities.h"


//extern bool sendCmd_Rob1;
//extern bool sendCmd_Rob2;
//robot_soccer::controldata cmdRob1;
//robot_soccer::controldata cmdRob2;
void Utilities::init(){
    idle();
}

void Utilities::rotate(double angle){
    printf("send command to move angle %f", angle);
}

void Utilities::move(Robot robot, Point dest, double des_theta){
    std::cout << "UTILITIES:: sending mov command" << std::endl;
    cmdRob1.cmdType = "mov";
    cmdRob1.x = robot.location.x;
    cmdRob1.y = robot.location.y;
    cmdRob1.theta = robot.theta;
    cmdRob1.x_cmd = dest.x;
    cmdRob1.y_cmd = dest.y;
    cmdRob1.theta_cmd = des_theta;
    sendCmd_Rob1 = true;
}

void Utilities::idle(){
    cmdRob1.cmdType = "idle";
    sendCmd_Rob1 = true;
}

void Utilities::dribble(){
    cmdRob1.cmdType = "dribble";
}

void Utilities::moveToPoint(Robot robot, Point point, double theta){
    move(robot, point, theta);
}

void Utilities::kick(double power, double theta){
    printf("ball kicked with power %f and angle %f\n", power, theta);
    //msg.cmdType = kick
}

void Utilities::moveToCenter(Robot robot, double theta){
    move(robot, center, theta);
}

void Utilities::followBall(FieldObject ball, Robot robot, double theta){
    printf("following ball\n");
    move(robot, ball.location, theta);
}

