#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <cstddef>

#include "dataclasses/point.h"
#include "dataclasses/fieldobject.h"
#include "dataclasses/robot.h"

namespace calc {
Point* directionToPredict(FieldObject* startobj, FieldObject* destobj, double time);
Point* directionToPoint(Point* startobj, Point* destobj);
Point* predictLocation(FieldObject* myobj, double time);
double angleDifference(double currentTheta, double desiredTheta);
}
#endif // MATHFUNCTIONS_H
