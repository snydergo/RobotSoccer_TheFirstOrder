#pragma once

#include "gameplay/fieldcoord.h"
#include "robot_soccer/controldata.h"
#include "robot_soccer/visiondata.h"
#include "dataclasses/gamestatus.h"
#include "mathfunctions.h"

// Type definitions
#define TICKS_PER_SEC 10

typedef short sem;

// GLOBALS
extern FieldCoord field;
extern Point center;
extern bool visionUpdated;
extern sem visionMsg_sem;
extern sem ctrlMsg_sem;
extern GameStatus visionStatus_msg;


//functions
bool pend(sem semaphore);
void post(sem semaphore);

