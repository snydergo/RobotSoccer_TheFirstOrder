#include "mathfunctions.h"

Point calc::directionToPredict(FieldObject startobj, FieldObject destobj, double time)
{
    return calc::directionToPoint(startobj.location,
                                  calc::predictLocation(destobj, time));
}

Point calc::directionToPoint(Point startobj, Point destobj)
{
    return Point(destobj.x - startobj.x,
                 destobj.y - startobj.y);
}


Point calc::predictLocation(FieldObject myobj, double time)
{
    return Point(myobj.location.x + myobj.velocity.x*time,
                 myobj.location.y + myobj.velocity.y*time);
}

bool calc::withinField(Point point)
{
    return (abs(point.x) < FIELD_XBORDER && abs(point.y) < FIELD_YBORDER);
}

Point calc::getNewPoint(Point point)
{
    if (withinField(point)) {
        return point;
    }else{
        if (abs(point.x) > FIELD_XBORDER) {
            point.x = point.x < 0 ? -FIELD_XBORDER+2: FIELD_XBORDER-2;
        }
        if (abs(point.y) > FIELD_YBORDER) {
            point.y = point.y < 0 ? -FIELD_YBORDER+2: FIELD_YBORDER-2;
        }
        //std::cout << "getNewPoint::Exit  point.x == " << point.x << " point.y == "<< point.y << std::endl;*/

        return point;
    }
}

double calc::angleDifference(double currentTheta, double desiredTheta)
{
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

Point calc::getVelocity(FieldObject newobj, FieldObject oldobj)
{
    double x_vel = newobj.location.x - oldobj.location.x;
    double y_vel = newobj.location.y - oldobj.location.y;
    if (abs(x_vel) < VELOCITY_THRESHOLD)
        x_vel = 0;

    if (abs(y_vel) < VELOCITY_THRESHOLD)
        y_vel = 0;

    return Point(x_vel, y_vel);
}

double calc::degToRad(double deg)
{
    return (deg*PI)/180;
}

double calc::radToDeg(double radians)
{
    return (radians*180)/PI;
}

double calc::getVectorAngle(Point vector)
{
    //need to first determine quadrant
    double vecAngle = 0;
    //top
    if (vector.y >= 0) {
         //top right
         if (vector.x > 0) {
            vecAngle = calc::radToDeg(atan2(vector.y, vector.x));
         }else{//top left
            vecAngle = calc::radToDeg(atan2(vector.y, vector.x));
         }
    }else {//bottom quadrant
         //bottom right
         if (vector.x > 0) {
            vecAngle = 360+calc::radToDeg(atan2(vector.y, vector.x));
         }else{//bottom left
            vecAngle = 360+calc::radToDeg(atan2(vector.y, vector.x));
         }
    }
    if (vecAngle > 180)
        vecAngle -= 360;

    return vecAngle;
}

//####PLAY THRESHOLD FUNCTIONS####//
bool calc::atLocation(Point robot, Point point, double err)
{
    if (robot.x != robot.x || robot.y != robot.y) {return false;}
//    std::cout << "point.x == " << std::to_string(point.x) << std::endl;
//    std::cout << "robot.x == " << std::to_string(robot.x) << std::endl;
    double xValues = robot.x-point.x;
//    std::cout << "xValues == " << std::to_string(xValues) << std::endl;
    xValues *= xValues;
    double yValues = robot.y-point.y;
//    std::cout << "yValues == " << std::to_string(yValues) << std::endl;
    yValues *= yValues;
    double distance = std::sqrt(xValues+yValues);
    std::cout << "distance == " << distance << std::endl;
    return (distance < err);
}

double calc::getDistance(Point robot, Point point)
{
    if (robot.x != robot.x || robot.y != robot.y) {return 0;}
//    std::cout << "point.x == " << std::to_string(point.x) << std::endl;
//    std::cout << "robot.x == " << std::to_string(robot.x) << std::endl;
    double xValues = robot.x-point.x;
//    std::cout << "xValues == " << std::to_string(xValues) << std::endl;
    xValues *= xValues;
    double yValues = robot.y-point.y;
//    std::cout << "yValues == " << std::to_string(yValues) << std::endl;
    yValues *= yValues;
    double distance = std::sqrt(xValues+yValues);
//    std::cout << "distance == " << distance << std::endl;
    return distance;
}

bool calc::withinPerimeter(Point robot, Point ball)
{
    if (robot.x != robot.x || robot.y != robot.y) {return false;}
//    std::cout << "point.x == " << std::to_string(ball.x) << std::endl;
//    std::cout << "robot.x == " << std::to_string(robot.x) << std::endl;
    double xValues = robot.x-ball.x;
//    std::cout << "xValues == " << std::to_string(xValues) << std::endl;
    xValues *= xValues;
    double yValues = robot.y-ball.y;
    std::cout << "yValues == " << std::to_string(yValues) << std::endl;
    yValues *= yValues;
    double perimeter = xValues+yValues;
    std::cout << "perimeter == " << perimeter << std::endl;
    return (perimeter < PERIMETER_ERR);
}

bool calc::atLocation(double robot_coord, double p_coord)
{
    double distance = std::abs(robot_coord - p_coord);
    return (distance < DISTANCE_ERR);
}

bool calc::ballFetched(Robot ally, FieldObject ball)
{
    //ball should be less than 4cm in front of robot x check
    //ball should be in the center of the robot with error of 4
    //angle should be towards the ball
    Point fetchballpoint(ball.location.x-FETCHBALL_OFFSET,ball.location.y);
    return calc::atLocation(ally.location, fetchballpoint, BALLFETCHED_ERR);
}

bool calc::ballAimed(Robot ally, FieldObject ball, Point enemyGoal)
{
    //for ball to be aimed. the robots angle needs to match
    //the angle created by the ball and the Goal. need to match
/*    Point allyBallVector = calc::directionToPoint(ally.location, ball.location);
    Point allyGoalVector = calc::directionToPoint(ally.location, enemyGoal);
    if (abs(ally.theta - calc::getVectorAngle(allyBallVector)) < GOAL_ANGLE_ERR &&
            abs(ally.theta - calc::getVectorAngle(allyGoalVector)) < GOAL_ANGLE_ERR &&
            abs(calc::getVectorAngle(allyBallVector)-calc::getVectorAngle(allyGoalVector)) < GOAL_ANGLE_ERR) {
        return true;
    }else
        return false;*/
    Point ballGoalDir = calc::directionToPoint(ball.location, enemyGoal);
    double ballGoalAngle = calc::getVectorAngle(ballGoalDir);

    if (abs(ballGoalAngle)< GOAL_ANGLE_ERR)
        return true;
    else
        return false;
}

bool calc::ballKicked(Robot ally, Point kp)
{
    return calc::atLocation(ally.location, kp);
}
