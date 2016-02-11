#include "mathfunctions.h"


Point calc::directionToPredict(FieldObject startobj, FieldObject destobj, double time){
    Point startPoint = startobj.location;
    Point predLoc = calc::predictLocation(destobj, time);
    return calc::directionToPoint(startPoint, predLoc);
}


Point calc::directionToPoint(Point startobj, Point destobj){
    return Point(destobj.x - startobj.x, destobj.y - startobj.y);
}


Point calc::predictLocation(FieldObject myobj, double time){
    double newX = myobj.location.x + myobj.velocity.x*time;
    double newY = myobj.location.y + myobj.velocity.y*time;
    return Point(newX,newY);
}


double calc::angleDifference(double currentTheta, double desiredTheta){
    double angle = currentTheta - desiredTheta;
    if(angle < -180){
        angle = -(360-abs(angle));
    }else if (angle > 180){
        angle = 360 - angle;
    }else{
        angle = -angle;
    }
    return angle;
}

