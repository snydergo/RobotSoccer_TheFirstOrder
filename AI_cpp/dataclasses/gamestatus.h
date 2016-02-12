#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <string>
#include <cstddef>

#include "point.h"
#include "robot.h"
#include "fieldobject.h"
#include "../gameplay/utilities.h"


class GameStatus{
public:
    Robot ally1;
    Robot ally2;
    Robot enemy1;
    Robot enemy2;
    FieldObject ball;

    //GameStatus function calls
    GameStatus(Robot ally1, Robot ally2, Robot enemy1, Robot enemy2, FieldObject ball)
    : ally1(ally1)
    , ally2(ally2)
    , enemy1(enemy1)
    , enemy2(enemy2)
    , ball(ball)
    {
    }
    GameStatus(){}

    std::string toString();
};


#endif // GAMESTATUS_H
