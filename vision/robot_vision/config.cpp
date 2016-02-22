#include "config.h"

namespace config {

cv::Point2f fieldCenter_px;
double cmPerPixelConversionFactor;
bool invertX;

int cropTop;
int cropBottom;
int cropLeft;
int cropRight;

HsvColorSubSpace ballColor;
int ballArea;

int teamRobotCount;
HsvColorSubSpace teamRobotPrimaryColor;
int teamRobotLargeArea;
int teamRobotSmallArea;

int opponentRobotCount;
HsvColorSubSpace opponentRobotPrimaryColor;
int opponentRobotLargeArea;
int opponentRobotSmallArea;


std::string cameraUrl;

} // namespace config
