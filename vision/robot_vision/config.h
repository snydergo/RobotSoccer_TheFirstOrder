#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/core/core.hpp"
#include "hsvcolorsubspace.h"

namespace config {

extern cv::Point2i fieldCenter_px;
extern double cmPerPixelConversionFactor;

extern int cropTop;
extern int cropBottom;
extern int cropLeft;
extern int cropRight;

extern HsvColorSubSpace ballColor;

extern int teamRobotCount;
extern HsvColorSubSpace teamRobotPrimaryColor;
extern HsvColorSubSpace teamRobotSecondaryColor;


extern int opponentRobotCount;
extern HsvColorSubSpace opponentRobotPrimaryColor;
extern HsvColorSubSpace opponentRobotSecondaryColor;


} // namespace config

#endif // CONFIG_H
