#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
/* ---------------------------------------------------------------------------
** mathfunctions.h
** contains needed calculatiosn for threshold functions where decisions are
** made whether a ball is kicked robot is in location and etc. It alco contains
** all of the necessary supporting vector and trig calcuations 
**
** Author: Glade Snyder
** -------------------------------------------------------------------------*/
#include "types.h"
#include "dataclasses/point.h"
#include "dataclasses/robot.h"
#include <math.h>
#include <cmath>
#include <iostream>

namespace calc {
    Point directionToPredict(FieldObject startobj, FieldObject destobj, double time);
    Point directionToPoint(Point startobj, Point destobj);
    Point predictLocation(FieldObject myobj, double time);
    double angleDifference(double currentTheta, double desiredTheta);
    Point getVelocity(FieldObject obj1, FieldObject obj2);
    double radToDeg(double radians);
    double getVectorAngle(Point vector);

    /*#### THRESHOLD PLAY FUNCTIONS ####*/
    bool withinField(Point point);
    Point getNewPoint(Point point);
    bool ballFetched(Robot ally, FieldObject ball);
    bool ballAimed(Robot ally, FieldObject ball, Point enemyGoal);
    bool ballKicked(Robot ally, Point kp);
    bool atLocation(Point robot, Point point);
    bool atLocation(double robot_coord, double p_coord);
    bool withinPerimeter(Point robot, Point ball);
}
#endif // MATHFUNCTIONS_H
