#include "subscriber_debugmsg.h"

void debugCallback(const robot_soccer::controldata& msg)
{
    std::cout << msg.x_cmd << std::endl;
    debugCmd = msg;
    newDebugCmd = true;
}
