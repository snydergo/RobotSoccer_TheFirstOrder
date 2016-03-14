/* ---------------------------------------------------------------------------
** subscriber_visionmsg.h
** vision subscriber that  receives data from vision and puts it into global
** variable visionStatus_msg and vision_msg
** Author: Glade Snyder
** -------------------------------------------------------------------------*/#include "ros/ros.h"
#include "robot_soccer/visiondata.h"
#include "../bookkeeping.h"

void visionCallback(const robot_soccer::visiondata& msg);
