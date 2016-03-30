/*in charge of all of the functions that creates the commands that
are sent to the robot to be executed*/
#include "utilities.h"
void assignCmd(RobotType tag, robot_soccer::controldata cmd)
{
    switch(tag) {
    case RobotType::ally1:
        cmdRob1 = cmd;
        sendCmd_Rob1 = true;
        break;
    case RobotType::ally2:
        cmdRob2 = cmd;
        sendCmd_Rob2 = true;
        break;
    default:
        throw ExceptionAI(errtype::RobotType);
        break;
    }
}

void Utilities::rotate(double angle)
{
    printf("send command to move angle %f", angle);
}

void Utilities::move(moveSpeed gvnSpeed, Robot robot, Point dest, double des_theta)
{
//    std::cout << "UTILITIES:: sending mov command" << std::endl;
    robot_soccer::controldata cmd;
    switch (gvnSpeed) {
    case moveSpeed::fast:
        cmd.cmdType = "movefast";
        break;
    case moveSpeed::med:
        cmd.cmdType = "moveslow";
        break;
    case moveSpeed::slow:
        cmd.cmdType = "moveslow";
        break;
    }
    cmd.x = robot.location.x;
    cmd.y = robot.location.y;
    cmd.theta = robot.theta;
    cmd.x_cmd = dest.x;
    cmd.y_cmd = dest.y;
    cmd.theta_cmd = des_theta;
    assignCmd(robot.tag, cmd);

}

void Utilities::idle(Robot robot)
{
    robot_soccer::controldata cmd;
    cmd.cmdType = "idle";
    assignCmd(robot.tag, cmd);
}

void Utilities::dribble(Robot robot)
{
    robot_soccer::controldata cmd;
    cmd.cmdType = "dribble";
    assignCmd(robot.tag, cmd);
}

void Utilities::moveToPoint(moveSpeed gvnSpeed, Robot robot, Point point, double theta)
{
    move(gvnSpeed, robot, point, theta);
}

void Utilities::initKick(Robot robot)
{
    robot_soccer::controldata cmd;
    cmd.cmdType = "kickinit";
    std::cout << "Utilities::initKick()" << std::endl;
    assignCmd(robot.tag, cmd);
}

void Utilities::uninitKick(Robot robot)
{
    robot_soccer::controldata cmd;
    cmd.cmdType = "kickuninit";
    assignCmd(robot.tag, cmd);
}

void Utilities::kick(Robot robot)
{
    printf("ball kicked\n");
    robot_soccer::controldata cmd;
    cmd.cmdType = "kick";
    assignCmd(robot.tag, cmd);
}

void Utilities::moveToCenter(Robot robot, double theta)
{
    move(moveSpeed::fast, robot, center, theta);
}

void Utilities::followBall(FieldObject ball, Robot robot, double theta)
{
    printf("following ball\n");
    move(moveSpeed::slow, robot, ball.location, theta);
}

