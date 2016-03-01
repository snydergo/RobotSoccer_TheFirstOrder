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
    ros::Time time;
    GameStatus()
    : ally1(robotType::ally1, Point(), Point(), 0)
    , ally2(robotType::ally2, Point(), Point(), 0)
    , enemy1(robotType::enemy1, Point(), Point(), 0)
    , enemy2(robotType::enemy2, Point(), Point(), 0)
    , ball(robotType::ball, Point(), Point())
    {
    }

    GameStatus& operator= (const robot_soccer::visiondata& arg);

    //GameStatus function calls
    std::string toString();
};


#endif // GAMESTATUS_H
