#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H


#include "dataclasses/point.h"
#include "dataclasses/robot.h"
#include <math.h>
#define ANGLE_ERR 10
#define BALLFETCHED_ERR 8

namespace calc {
    Point directionToPredict(FieldObject startobj, FieldObject destobj, double time);
    Point directionToPoint(Point startobj, Point destobj);
    Point predictLocation(FieldObject myobj, double time);
    double angleDifference(double currentTheta, double desiredTheta);
    Point getVelocity(FieldObject obj1, FieldObject obj2);
    double radToDeg(double radians);
    double getVectorAngle(Point vector);
    bool ballFetched(Robot ally, FieldObject ball);
}
#endif // MATHFUNCTIONS_H
