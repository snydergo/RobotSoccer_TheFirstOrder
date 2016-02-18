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

Point calc::getVelocity(FieldObject newobj, FieldObject oldobj){
    double x_vel = newobj.location.x - oldobj.location.x;
    double y_vel = newobj.location.y - oldobj.location.y;
    return Point(x_vel, y_vel);
}

double calc::radToDeg(double radians){
    return(radians*180)/M_PI;
}

double calc::getVectorAngle(Point vector){
    //need to first determine quadrant
    double vecAngle = 0;
    //top
    if(vector.y > 0){
         //top right
         if(vector.x > 0){
            vecAngle = calc::radToDeg(atan2(vector.y, vector.x));
         }else{//top left
            vecAngle = 180+calc::radToDeg(atan2(vector.y, vector.x));
         }
    }else {//bottom quadrant
         //bottom right
         if(vector.x > 0){
            vecAngle = 360+calc::radToDeg(atan2(vector.y, vector.x));
         }else{//bottom left
            vecAngle = 180+calc::radToDeg(atan2(vector.y, vector.x));
         }
    }
}

bool calc::ballFetched(Robot ally, FieldObject ball){
    Point dist = calc::directionToPoint(ally.location, ball.location);
    //ball should be less than 4cm in front of robot x check
    //ball should be in the center of the robot with error of 4
    //angle should be towards the ball
    if(dist.x > BALLFETCHED_ERR || dist.y > BALLFETCHED_ERR ||
            abs(calc::getVectorAngle(dist) - ally.theta) > ANGLE_ERR) {
        return false;
    }

}
