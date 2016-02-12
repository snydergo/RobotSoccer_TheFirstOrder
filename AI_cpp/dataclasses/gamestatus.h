#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <string>

#include "robot.h"
#include "fieldobject.h"

#include "robot_soccer/visiondata.h"

class GameStatus{
public:
    Robot ally1;
    Robot ally2;
    Robot enemy1;
    Robot enemy2;
    FieldObject ball;

    GameStatus()
    : ally1("ally1", Point(), Point(), 0, robotType::ally1)
    , ally2("ally2", Point(), Point(), 0, robotType::ally2)
    , enemy1("enemy1", Point(), Point(), 0, robotType::enemy1)
    , enemy2("enemy2", Point(), Point(), 0, robotType::enemy2)
    , ball("ball", Point(), Point())
    {
    }

    GameStatus& operator= (const robot_soccer::visiondata& arg);

    //GameStatus function calls
    std::string toString();
};


#endif // GAMESTATUS_H
