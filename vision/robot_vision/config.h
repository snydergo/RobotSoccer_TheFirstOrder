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

const HsvColorSubSpace green({60,90},{30,145},{180,255});
const HsvColorSubSpace blue({80,100},{30,235},{170,255});
const HsvColorSubSpace purple({120,140},{30,90},{200,250});
const HsvColorSubSpace red({175,179},{30,200},{200,255});
const HsvColorSubSpace orange({0,30},{50,190},{150,255});
const HsvColorSubSpace green({140,175},{30,150},{120,255});
const HsvColorSubSpace yellow({10,45},{0,90},{225,255});




} // namespace config

#endif // CONFIG_H
