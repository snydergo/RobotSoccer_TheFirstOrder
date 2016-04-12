#include "subscriber_debugmsg.h"

void debugCallback(const robot_soccer::controldata& msg)
{
    debugCmd = msg;
    newDebugCmd = true;
}
