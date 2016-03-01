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

/*GameStatus::GameStatus(){
    this->ally1.location = NULL;
    this->ally1.tag = "ally1";
    this->ally1.theta = 0;
    this->ally1.velocity = NULL;
    this->ally2.location = NULL;
    this->ally2.tag = "ally2";
    this->ally2.theta = 0;
    this->ally2.velocity = NULL;
    this->enemy1.location = NULL;
    this->enemy1.tag = "enemy1";
    this->enemy1.theta = 0;
    this->enemy1.velocity = NULL;
    this->enemy2.location = NULL;
    this->enemy2.tag = "enemy2";
    this->enemy2.theta = 0;
    this->enemy2.velocity = NULL;
    this->ball.location = NULL;
    this->ball.tag = NULL;
    this->ball.velocity = NULL;

}*/

std::string GameStatus::toString() {
    return "null";
}
