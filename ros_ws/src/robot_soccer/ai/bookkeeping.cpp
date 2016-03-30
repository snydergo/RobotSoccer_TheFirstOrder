#include "bookkeeping.h"

//## GLOBAL DEBUG ##//
std::ofstream stream::stateStatus;
std::ofstream stream::info;
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

Robot fieldget::getRobot(RobotType type)
{
    switch (type) {
    case RobotType::ally1:  return field.currentStatus.ally1;
    case RobotType::ally2:  return field.currentStatus.ally2;
    case RobotType::enemy1: return field.currentStatus.enemy1;
    case RobotType::enemy2: return field.currentStatus.enemy2;
    default:
        printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
        return Robot(RobotType::none, Point(), Point(), 0.0);
    }
}

Point fieldget::getRobotLoc(RobotType type)
{
    return fieldget::getRobot(type).location;
}


//#### BOOK KEEPING CALC FUNCTIONS ####//

bool bkcalc::ballKickZone(RobotType type)
{
    switch (type) {
    case RobotType::ally1:
    {
        std::cout << "ballkickZone::ally1" << std::endl;
        Point perCenter(field.currentStatus.ally1.location.x+PERIMETER_XOFFSET,field.currentStatus.ally1.location.y);
        return calc::withinPerimeter(perCenter, field.currentStatus.ball.location);
    }
    case RobotType::ally2:
        std::cout << "ballkickZone::ally2" << std::endl;
        return calc::withinPerimeter(field.currentStatus.ally2.location, field.currentStatus.ball.location);
    default:
        printf("bookkeeping:atLocation:: ERR you didn't provide a valid robot\n");
        return false;
    }
}

double bkcalc::getAngleTo(RobotType type, Point point)
{
    return calc::getVectorAngle(calc::directionToPoint(fieldget::getRobotLoc(type), point));
}

Point bkcalc::kickPoint(RobotType type)
{
    Point kickerLoc = fieldget::getRobotLoc(type);
    Point dir = calc::directionToPoint(kickerLoc,field.currentStatus.ball.location);
    Point result = Point(kickerLoc.x+KICK_FACTOR*dir.x,kickerLoc.y+KICK_FACTOR*dir.y);

    if(!calc::withinField(result)) {
        result = calc::getNewPoint(result);
    }
    return result;
}

bool bkcalc::ballAimed(RobotType type)
{
    return calc::ballAimed(fieldget::getRobot(type), field.currentStatus.ball, enemyGoal);
}

bool bkcalc::ballThreat()
{
    //   std::cout << "####bkcalc::ballThreat####" << std::endl;
    return ((field.currentStatus.ball.velocity.x < 0 && field.currentStatus.ball.location.x < -20));
}
