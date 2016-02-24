#include "bookkeeping.h"

Point center(0,0);
Point enemyGoal(GOAL_XLOCATION,0);
Point allyGoal(-GOAL_XLOCATION,0);
Point start1Location(0,0);
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

bool bkcalc::ballKicked(robotType type, Point kp){
    bool ballkicked = false;
    switch(type){
        case robotType::ally1:
            ballkicked = calc::ballKicked(field.currentStatus.ally1, kp);
            break;
        case robotType::ally2:
            ballkicked = calc::ballKicked(field.currentStatus.ally2, kp);
            break;
        default:
            printf("bookkeeping:ballKicked: ERR you didn't provide a valid robot\n");
            break;
    }
    return ballkicked;
}

bool bkcalc::ballFetched(robotType type){
    bool ballfetched = false;
    switch(type){
        case robotType::ally1:
            ballfetched = calc::ballFetched(field.currentStatus.ally1, field.currentStatus.ball);
            break;
        case robotType::ally2:
            ballfetched = calc::ballFetched(field.currentStatus.ally2, field.currentStatus.ball);
            break;
        default:
            printf("bookkeeping:ballfetched: ERR you didn't provide a valid robot\n");
            break;
    }
    return ballfetched;
}

double bkcalc::getAngleTo(robotType type, Point point){
    Point dir;
    switch(type){
        case robotType::ally1:
            dir = calc::directionToPoint(field.currentStatus.ally1.location, point);
            break;
        case robotType::ally2:
            dir = calc::directionToPoint(field.currentStatus.ally2.location, point);
            break;
        default:
            printf("bookkeeping:ballfetched: ERR you didn't provide a valid robot\n");
            break;
    }
    return calc::getVectorAngle(dir);
}

Point bkcalc::kickPoint(robotType type){
    Point dir;
    Point result;
    Point kickerLoc;
    switch(type){
        case robotType::ally1:
            kickerLoc = field.currentStatus.ally1.location;
            break;
        case robotType::ally2:
            kickerLoc = field.currentStatus.ally2.location;
            break;
        default:
            printf("bookkeeping:ballfetched: ERR you didn't provide a valid robot\n");
            break;
    }
    dir = calc::directionToPoint(kickerLoc,field.currentStatus.ball.location);
    result = Point(kickerLoc.x+KICK_FACTOR*dir.x,kickerLoc.y+KICK_FACTOR*dir.y);
    return result;
}

bool bkcalc::ballThreat(){
   return(field.currentStatus.ball.velocity.x < 0 &&
        field.currentStatus.ball.location.x < 0);
}
