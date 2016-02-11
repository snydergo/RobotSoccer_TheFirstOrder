#ifndef GAMESTATUS_H
#define GAMESTATUS_H


#include <string>
#include <cstddef>
#include "point.h"
#include "robot.h"
#include "fieldobject.h"
#include "utilities.h"

class GameStatus{
public:
    Robot* ally1 = NULL;
    Robot* ally2 = NULL;
    Robot* enemy1 = NULL;
    Robot* enemy2 = NULL;
    FieldObject* ball = NULL;

    //GameStatus function calls
    GameStatus(Robot* ally1, Robot *ally2, Robot *enemy1, Robot *enemy2, FieldObject *ball);
    std::string toString();
};


#endif // GAMESTATUS_H
