#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/core/core.hpp"
#include "hsvcolorsubspace.h"

using cv::Point2f;

namespace config {

extern Point2f fieldCenter_px;
extern double cmPerPixelConversionFactor;
extern bool invertX;

extern bool homeIsInverted;

extern bool trackRobot;

extern int cropTop;
extern int cropBottom;
extern int cropLeft;
extern int cropRight;

extern HsvColorSubSpace ballColor;
extern int ballArea;

extern int allyRobotCount;
extern HsvColorSubSpace allyRobot1Color;
extern HsvColorSubSpace allyRobot2Color;


extern int enemyRobotCount;
extern HsvColorSubSpace enemyRobot1Color;
extern HsvColorSubSpace enemyRobot2Color;

extern std::string cameraUrl;


extern const HsvColorSubSpace green;
extern const HsvColorSubSpace blue;
extern const HsvColorSubSpace purple;
extern const HsvColorSubSpace red;
extern const HsvColorSubSpace orange;
extern const HsvColorSubSpace pink;
extern const HsvColorSubSpace yellow;




} // namespace config

#endif // CONFIG_H
