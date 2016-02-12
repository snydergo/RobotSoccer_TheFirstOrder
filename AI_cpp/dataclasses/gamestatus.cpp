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
}

std::string GameStatus::toString() {
    return "null";
}
