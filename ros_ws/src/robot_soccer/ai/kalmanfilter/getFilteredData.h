/* ---------------------------------------------------------------------------
** getFilteredData.h
** used to send data to python filter node through the rostopic inputfilter
** and then output data is published to output filter
**
** Author: Glade Snyder
** -------------------------------------------------------------------------*/
#include "ros/ros.h"
#include "robot_soccer/visiondata.h"
#include "../bookkeeping.h"

void filterCallback(const robot_soccer::visiondata& msg);
