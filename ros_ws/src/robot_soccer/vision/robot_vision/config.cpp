#include "config.h"

namespace config {

std::string cameraUrl;
cv::Point2f fieldCenter_px;
double cmPerPixelConversionFactor;
bool invertX;
bool homeIsInverted;

bool trackRobot;
bool showVideo;

int cropTop;
int cropBottom;
int cropLeft;
int cropRight;

HsvColorSubSpace ballColor;
int ballArea;

int allyRobotCount;
HsvColorSubSpace allyRobot1Color;
HsvColorSubSpace allyRobot2Color;

int enemyRobotCount;
HsvColorSubSpace enemyRobot1Color;
HsvColorSubSpace enemyRobot2Color;

const HsvColorSubSpace green({50,84},{30,120},{180,255});
const HsvColorSubSpace blue({80,100},{30,255},{190,255});
const HsvColorSubSpace purple({110,140},{30,90},{180,255});
const HsvColorSubSpace red({175,179},{30,200},{200,255});
const HsvColorSubSpace orange({0,30},{50,255},{210,255});
const HsvColorSubSpace pink({160,175},{30,150},{200,255});
const HsvColorSubSpace yellow({10,45},{0,90},{225,255});

} // namespace config
