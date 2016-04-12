#ifndef TYPES_H
#define TYPES_H
/* ---------------------------------------------------------------------------
** getFilteredData.h
** Contains all of the #defines that are used for thresholding, clocks and etc
**
** Author: Glade Snyder
** -------------------------------------------------------------------------*/
//## FIELD PARAMETERS ##//
#define GOAL_XLOCATION          150 //160 real 130 gazebo
#define GOAL_WIDTH              60
#define GOAL_RADIUS             (GOAL_WIDTH/2)
#define GOAL_ANGLE_ERR          15
#define FIELD_XBORDER           165
#define FIELD_YBORDER           100
#define PI                      3.141592653
#define STARTCENTER             60

//## GAME CONTROL FLAGS ##//
#define STOP      1
#define START     2
#define MARK      4

//## THRESHOLD DEFINITIONS ##//
#define ANGLE_ERR               20
#define AIM_MAX_CNT             200
#define AIM_BALL_DIST           20
#define KICK_INVALID_DIST       40
#define DISTANCE_ERR            6 // this is an error of 6cm
#define MVSPD_FAST_THRESH       40
#define PERIMETER_ERR           100
#define PERIMETER_XOFFSET       5
#define PERIMETER_YOFFSET       3
#define KICK_FACTOR             3
#define BALLFETCHED_ERR         10
#define FETCHBALL_OFFSET        20
#define IN_GOAL_X               175
#define IN_GOAL_Y               (GOAL_WIDTH/2)
#define SPLIT_OFFSET            0
#define SPLIT_WAIT_XOFFSET      60
#define SPLIT_WAIT_Y            (FIELD_YBORDER/3)
#define KICKPOINTTHRESH         10
//## TIMING DEFINES ##//
#define TICKS_PER_SEC           40
#define VELOCITY_THRESHOLD      .000001
#define VELOCITY_DIVIDER        (1/TICKS_PER_SEC)

//# FILTER DEFINES #//
#define DOSAMPLE        0
#define BETASMOOTH0     .5 //main filter
#define BETASMOOTH1     .35
#define BETASMOOTH2     .5
#define BETASMOOTH3     .75
#define SAMPLENUM       300

//## NEEDED TYPES & OBJECTS ##//
enum class RobotType {ally1, ally2, enemy1, enemy2, ball, object, none};
enum class side {left, right, pos, neg, none};
enum class moveSpeed {fast, med, slow};
#endif // TYPES_H
