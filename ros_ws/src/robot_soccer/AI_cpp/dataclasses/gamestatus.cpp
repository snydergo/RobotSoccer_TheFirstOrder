#include "gamestatus.h"

GameStatus& GameStatus::operator= (const robot_soccer::visiondata& arg) {
    ally1.location.x = arg.tm0_x;
    ally1.location.y = arg.tm0_y;
    ally1.theta = arg.tm0_w;
    ally2.location.x = arg.tm1_x;
    ally2.location.y = arg.tm1_y;
    ally2.theta = arg.tm1_w;
    enemy1.location.x = arg.op0_x;
    enemy1.location.y = arg.op0_y;
    enemy1.theta = arg.op0_w;
    enemy2.location.x = arg.op1_x;
    enemy2.location.y = arg.op1_y;
    enemy2.theta = arg.op1_w;
    ball.location.x = arg.ball_x;
    ball.location.y = arg.ball_y;
    time = arg.sys_time;
}

GameStatus::operator robot_soccer::visiondata() const
{
    robot_soccer::visiondata msg;
    msg.tm0_x = ally1.location.x;
    msg.tm0_y = ally1.location.y;
    msg.tm0_w = ally1.theta;
    msg.tm1_x = ally2.location.x ;
    msg.tm1_y = ally2.location.y;
    msg.tm1_w = ally2.theta;
    msg.op0_x = enemy1.location.x;
    msg.op0_y = enemy1.location.y;
    msg.op0_w = enemy1.theta;
    msg.op1_x = enemy2.location.x;
    msg.op1_y = enemy2.location.y;
    msg.op1_w = enemy2.theta;
    msg.ball_x = ball.location.x;
    msg.ball_y = ball.location.y;
    msg.sys_time = time;
    return msg;
}

std::string GameStatus::toString() {
    return "null";
}
