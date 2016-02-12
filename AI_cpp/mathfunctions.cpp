#include "mathfunctions.h"

Point calc::directionToPredict(FieldObject startobj, FieldObject destobj, double time){
    return calc::directionToPoint(startobj.location,
                                  calc::predictLocation(destobj, time));
}

Point calc::directionToPoint(Point startobj, Point destobj){
    return Point(destobj.x - startobj.x,
                 destobj.y - startobj.y);
}


Point calc::predictLocation(FieldObject myobj, double time){
    return Point(myobj.location.x + myobj.velocity.x*time,
                 myobj.location.y + myobj.velocity.y*time);
}

double calc::angleDifference(double currentTheta, double desiredTheta){
    double angle = currentTheta - desiredTheta;
    if (angle < -180) {
        angle = -(360-abs(angle));
    } else if (angle > 180) {
        angle = 360 - angle;
    } else {
        angle = -angle;
    }
    return angle;
}

