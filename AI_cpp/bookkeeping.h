#pragma once

#include "gameplay/fieldcoord.h"
#include "robot_soccer/controldata.h"
#include "robot_soccer/visiondata.h"
#include "dataclasses/gamestatus.h"
#include "mathfunctions.h"

// Type definitions
#define TICKS_PER_SEC 10

enum class msgType {ally1, ally2, none};
typedef short sem;

// GLOBALS
extern FieldCoord field;
extern Point center;
extern bool visionUpdated;
extern sem visionMsg_sem;
extern sem ctrlMsg_sem;
extern GameStatus visionStatus_msg;

//cmd globals
extern bool sendCmd_Rob1;
extern bool sendCmd_Rob2;
extern robot_soccer::controldata cmdRob1;
extern robot_soccer::controldata cmdRob2;
//functions
bool pend(sem semaphore);
void post(sem semaphore);

