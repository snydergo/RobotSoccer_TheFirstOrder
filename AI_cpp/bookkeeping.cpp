#include "bookkeeping.h"

Point center = Point(0,0);
Point enemyGoal = Point(GOAL_XLOCATION,0);
Point allyGoal = Point(-GOAL_XLOCATION,0);
Point start1Location = Point(-GOAL_XLOCATION,0);
FieldCoord field;
bool visionUpdated;
GameStatus visionStatus_msg;

bool sendCmd_Rob1 = true;
bool sendCmd_Rob2 = false;
robot_soccer::controldata cmdRob1;
robot_soccer::controldata cmdRob2;

//Helper Functions
bool bkcalc::atLocation(robotType type, Point point){
    bool atlocation = false;
    switch(type){
        case robotType::ally1:
            atlocation = calc::atLocation(field.currentStatus.ally1.location, point);
            break;
        case robotType::ally2:
            atlocation = calc::atLocation(field.currentStatus.ally2.location, point);
            break;
        default:
            printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
            break;
    }
    return atlocation;
}

bool bkcalc::ballKicked(robotType type){
    bool ballkicked = false;
    switch(type){
        case robotType::ally1:
            ballkicked = calc::ballKicked(field.currentStatus.ally1, field.currentStatus.ball);
            break;
        case robotType::ally2:
            ballkicked = calc::ballKicked(field.currentStatus.ally2, field.currentStatus.ball);
            break;
        default:
            printf("bookkeeping:ballKicked: ERR you didn't provide a valid robot\n");
            break;
    }
    return ballkicked;
}
