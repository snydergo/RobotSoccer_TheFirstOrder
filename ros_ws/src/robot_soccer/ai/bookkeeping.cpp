#include "bookkeeping.h"

//## FIELD INFO & OBJECTS ##//
Point center(0,0);
Point enemyGoal(GOAL_XLOCATION,0);
Point allyGoal(-GOAL_XLOCATION,0);
Point start1Location(-40,0);
Point start2Location(-40,0);
FieldCoord field;
bool iskick = false;

//## VISION DATA ##//
bool visionUpdated;
robot_soccer::visiondata vision_msg;
GameStatus visionStatus_msg;

//## CONTROL DATA ##//
bool sendCmd_Rob1(true);
bool sendCmd_Rob2(false);
robot_soccer::controldata cmdRob1;
robot_soccer::controldata cmdRob2;

//## DEBUG DATA ##//
bool newDebugCmd = false;
robot_soccer::controldata debugCmd;


//## FILTER DATA ##//
GameStatus predicted;
bool predictedUpdated(false);

//#### FIELDGET FUNCTIONS ####//
FieldObject fieldget::getBall()
{
    return field.currentStatus.ball;
}

Point fieldget::getBallLoc()
{
    return field.currentStatus.ball.location;
}

Robot fieldget::getRobot(robotType type)
{
    switch (type) {
        case robotType::ally1:
            return field.currentStatus.ally1;
        case robotType::ally2:
            return field.currentStatus.ally2;
        case robotType::enemy1:
            return field.currentStatus.enemy1;
        case robotType::enemy2:
            return field.currentStatus.enemy2;
        default:
            printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
            return Robot(robotType::none, Point(), Point(), 0.0);
    }
}

Point fieldget::getRobotLoc(robotType type)
{
    return fieldget::getRobot(type).location;
}


//#### BOOK KEEPING CALC FUNCTIONS ####//
bool bkcalc::atLocation(robotType type, Point point)
{

    switch (type) {
        case robotType::ally1:
            return calc::atLocation(field.currentStatus.ally1.location, point);
        case robotType::ally2:
            return calc::atLocation(field.currentStatus.ally2.location, point);
        default:
            printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
            return false;
    }
}

bool bkcalc::ballKicked(robotType type, Point kp)
{
    switch (type) {
        case robotType::ally1:
            return calc::ballKicked(field.currentStatus.ally1, kp);
        case robotType::ally2:
            return calc::ballKicked(field.currentStatus.ally2, kp);
        default:
            printf("bookkeeping:ballKicked: ERR you didn't provide a valid robot\n");
            return false;
    }
}

bool bkcalc::ballFetched(robotType type)
{
    switch (type) {
        case robotType::ally1:
            std::cout << "ROBOT1" << std::endl;
            return calc::ballFetched(field.currentStatus.ally1, field.currentStatus.ball);
        case robotType::ally2:
            return calc::ballFetched(field.currentStatus.ally2, field.currentStatus.ball);
        default:
            printf("bookkeeping:ballfetched: ERR you didn't provide a valid robot %d\n", type);
            return false;
    }
}

bool bkcalc::ballKickZone(robotType type)
{
    switch (type) {
        case robotType::ally1:
        {
            std::cout << "ballkickZone::ally1" << std::endl;
            Point perCenter(field.currentStatus.ally1.location.x+PERIMETER_XOFFSET,field.currentStatus.ally1.location.y);
            return calc::withinPerimeter(perCenter, field.currentStatus.ball.location);
        }
        case robotType::ally2:
            std::cout << "ballkickZone::ally2" << std::endl;
            return calc::withinPerimeter(field.currentStatus.ally2.location, field.currentStatus.ball.location);
        default:
            printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
            return false;
    }
}

double bkcalc::getAngleTo(robotType type, Point point)
{
    Point dir;
    switch (type) {
        case robotType::ally1:
            dir = calc::directionToPoint(field.currentStatus.ally1.location, point);
            break;
        case robotType::ally2:
            dir = calc::directionToPoint(field.currentStatus.ally2.location, point);
            break;
        default:
            printf("bookkeeping:getAngleTo: ERR you didn't provide a valid robot %d\n", type);
            break;
    }
    return calc::getVectorAngle(dir);
}

Point bkcalc::kickPoint(robotType type)
{
    Point result;
    /*Point kickerLoc;
    Point dir;
    switch(type) {
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
    result = Point(kickerLoc.x+KICK_FACTOR*dir.x,kickerLoc.y+KICK_FACTOR*dir.y);*/
    result = field.currentStatus.ball.location;
    if(!calc::withinField(result)) {
        result = calc::getNewPoint(result);
    }
    return result;
}

bool bkcalc::ballAimed(robotType type)
{
    Robot kicker;
    switch (type) {
        case robotType::ally1:
            kicker = field.currentStatus.ally1;
            break;
        case robotType::ally2:
            kicker = field.currentStatus.ally2;
            break;
        default:
            printf("bookkeeping:ballfetched: ERR you didn't provide a valid robot\n");
            break;
    }
    return calc::ballAimed(kicker, field.currentStatus.ball, enemyGoal);
}

bool bkcalc::ballThreat() {
//   std::cout << "####bkcalc::ballThreat####" << std::endl;
return ((field.currentStatus.ball.velocity.x < 0 && field.currentStatus.ball.location.x < -20));
}
