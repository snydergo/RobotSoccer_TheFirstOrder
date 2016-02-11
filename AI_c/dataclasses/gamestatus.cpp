#include "gamestatus.h"

//GameStatus Object functions
GameStatus::GameStatus(Robot* ally1, Robot *ally2, Robot *enemy1, Robot *enemy2, FieldObject *ball){
    this->ally1 = ally1;
    this->ally2 = ally2;
    this->enemy1 = enemy1;
    this->enemy2 = enemy2;
    this->ball = ball;
}

std::string GameStatus::toString(){
    return "null";
}
