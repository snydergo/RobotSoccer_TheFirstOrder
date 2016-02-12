#pragma once

#include "gameplay/fieldcoord.h"

// Type definitions
typedef short sem;

extern Point center;

// GLOBALS
extern FieldCoord field;
extern bool visionUpdated;
extern sem visionMsg_sem;
extern sem ctrlMsg_sem;
extern GameStatus visionStatus_msg;

//functions
bool pend(sem semaphore);
void post(sem semaphore);
