#pragma once
/* ---------------------------------------------------------------------------
** bookkeeping.h
** contains globals for the entire game. Also wrapper functions for the
** mathfunctions file and uses the actual robots at play and being tested
**
** Author: Glade Snyder
** -------------------------------------------------------------------------*/
#include "gameplay/fieldcoord.h"
#include "robot_soccer/controldata.h"
#include "robot_soccer/visiondata.h"
#include "dataclasses/gamestatus.h"
#include "mathfunctions.h"
#include <fstream>
//Outputs

/*namespace stream{
    std::ofstream stateStatus;
    std::ofstream info;
}*/


// GLOBALS
extern FieldCoord field;
extern Point center;
extern Point enemyGoal;
extern Point allyGoal;
extern Point start1Location;
extern Point start2Location;

//vision globals
extern bool visionUpdated;
extern robot_soccer::visiondata vision_msg;
extern GameStatus visionStatus_msg;

//cmd globals
extern bool sendCmd_Rob1;
extern bool sendCmd_Rob2;
extern robot_soccer::controldata cmdRob1;
extern robot_soccer::controldata cmdRob2;

//debug globals
extern bool newDebugCmd;
extern robot_soccer::controldata debugCmd;

//filter globals
extern GameStatus predicted;
extern GameStatus predictedPositions_msg;
extern bool predictedUpdated;

//functions
namespace fieldget {
    FieldObject* getBall();
    Robot* getRobot(robotType type);
    Point getBallLoc();
    Point getRobotLoc(robotType type);
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

