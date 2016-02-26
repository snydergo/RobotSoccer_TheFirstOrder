#ifndef TYPES_H
#define TYPES_H

//## FIELD PARAMETERS ##//
#define GOAL_XLOCATION 160
#define GOAL_WIDTH 60
#define GOAL_RADIUS GOAL_WIDTH/2
#define FIELD_XBORDER 180
#define FIELD_YBORDER 88
#define PI 3.141592653

//## THRESHOLD DEFINITIONS ##//
#define ANGLE_ERR 10
#define DISTANCE_ERR 16 // this is an error of 4cm
#define PERIMETER_ERR 124
#define KICK_FACTOR 3
#define BALLFETCHED_ERR 20
#define FETCHBALL_OFFSET 20

//## TIMING DEFINES ##//
#define TICKS_PER_SEC 50
#define VELOCITY_THRESHOLD .000001
#define VELOCITY_DIVIDER (1/TICKS_PER_SEC)

//# FILTER DEFINES #//
#define DOSAMPLE    0
#define BETASMOOTH0 .5 //main filter
#define BETASMOOTH1 .35
#define BETASMOOTH2 .5
#define BETASMOOTH3 .75
#define SAMPLENUM 300

//## NEEDED TYPES & OBJECTS ##//
enum class robotType {ally1, ally2, enemy1, enemy2, ball, object, none};


#endif // TYPES_H
