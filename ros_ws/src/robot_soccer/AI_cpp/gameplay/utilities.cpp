/*in charge of all of the functions that creates the commands that
are sent to the robot to be executed*/
#include "utilities.h"

void Utilities::init(){
    idle();
}

void Utilities::rotate(double angle){
    printf("send command to move angle %f", angle);
}

void Utilities::move(Robot robot, Point dest, double des_theta){
    std::cout << "UTILITIES:: sending mov command" << std::endl;
    robot_soccer::controldata cmd;
    cmd.cmdType = "mov";
    cmd.x = robot.location.x;
    cmd.y = robot.location.y;
    cmd.theta = robot.theta;
    cmd.x_cmd = dest.x;
    cmd.y_cmd = dest.y;
    cmd.theta_cmd = des_theta;
    sendCmd_Rob1 = true;

    switch(robot.tag){
        case robotType::ally1:
             cmdRob1 = cmd;
            break;
        case robotType::ally2:
             cmdRob2 = cmd;
            break;
        default:
            throw ExceptionAI(errtype::robotType);
            break;
    }
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
    cmdRob1.cmdType = "kick";
}

void Utilities::moveToCenter(Robot robot, double theta){
    move(robot, center, theta);
}

void Utilities::followBall(FieldObject ball, Robot robot, double theta){
    printf("following ball\n");
    move(robot, ball.location, theta);
}

