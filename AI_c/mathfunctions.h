#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <cstddef>

#include "point.h"
#include "fieldobject.h"
#include "robot.h"

namespace calc {
Point* directionToPredict(FieldObject* startobj, FieldObject* destobj, double time);
Point* directionToPoint(Point* startobj, Point* destobj);
Point* predictLocation(FieldObject* myobj, double time);
double angleDifference(double currentTheta, double desiredTheta);
}
#endif // MATHFUNCTIONS_H
