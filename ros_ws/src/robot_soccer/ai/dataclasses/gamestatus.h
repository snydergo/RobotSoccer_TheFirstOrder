/*Data structure that contains all of the Robots and Field Objects
on a given field. All of which have a velocity, Location and Tag
It will eventually need to also count the score*/
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
    : ally1(RobotType::ally1, Point(), Point(), 0)
    , ally2(RobotType::ally2, Point(), Point(), 0)
    , enemy1(RobotType::enemy1, Point(), Point(), 0)
    , enemy2(RobotType::enemy2, Point(), Point(), 0)
    , ball(RobotType::ball, Point(), Point())
    {
    }

    GameStatus& operator= (const robot_soccer::visiondata& arg);

    //GameStatus function calls
    std::string toString();

    operator robot_soccer::visiondata() const;
};


#endif // GAMESTATUS_H
