#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/core/core.hpp"
#include "hsvcolorsubspace.h"

using cv::Point2f;

namespace config {

extern Point2f fieldCenter_px;
extern double cmPerPixelConversionFactor;
extern bool invertX;

extern int cropTop;
extern int cropBottom;
extern int cropLeft;
extern int cropRight;

extern HsvColorSubSpace ballColor;
extern int ballArea;

extern int teamRobotCount;
extern HsvColorSubSpace teamRobotPrimaryColor;
extern int teamRobotLargeArea;
extern int teamRobotSmallArea;


extern int opponentRobotCount;
extern HsvColorSubSpace opponentRobotPrimaryColor;
extern HsvColorSubSpace opponentRobotSecondaryColor;
extern int opponentRobotLargeArea;
extern int opponentRobotSmallArea;

extern std::string cameraUrl;


} // namespace config

#endif // CONFIG_H
