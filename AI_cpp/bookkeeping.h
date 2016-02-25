#pragma once

#include "gameplay/fieldcoord.h"
#include "robot_soccer/controldata.h"
#include "robot_soccer/visiondata.h"
#include "dataclasses/gamestatus.h"
#include "mathfunctions.h"

// Type definitions



// GLOBALS
extern FieldCoord field;
extern Point center;
extern Point enemyGoal;
extern Point allyGoal;
extern Point start1Location;
extern bool visionUpdated;
extern GameStatus visionStatus_msg;

//cmd globals
extern bool sendCmd_Rob1;
extern bool sendCmd_Rob2;
extern robot_soccer::controldata cmdRob1;
extern robot_soccer::controldata cmdRob2;
//functions
namespace fieldget {
    FieldObject* getBall();
    Robot* getRobot();
}

namespace bkcalc{
    extern double getAngleTo(robotType type, Point point);
    extern bool atLocation(robotType type, Point point);
    extern bool ballKicked(robotType type, Point kp);
    extern bool ballKickZone(robotType type);
    extern bool ballFetched(robotType type);
    extern bool ballThreat();
    extern bool ballAimed(robotType type);
    extern Point kickPoint(robotType type);
}


