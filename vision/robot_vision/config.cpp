#include "config.h"

namespace config {

cv::Point2i fieldCenter_px;
double cmPerPixelConversionFactor;

int cropTop;
int cropBottom;
int cropLeft;
int cropRight;

HsvColorSubSpace ballColor;

int teamRobotCount;
HsvColorSubSpace teamRobotPrimaryColor;
HsvColorSubSpace teamRobotSecondaryColor;

int opponentRobotCount;
HsvColorSubSpace opponentRobotPrimaryColor;
HsvColorSubSpace opponentRobotSecondaryColor;

} // namespace config
