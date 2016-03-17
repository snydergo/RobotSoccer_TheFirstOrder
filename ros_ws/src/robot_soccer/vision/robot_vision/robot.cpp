
#include "robot.h"
#include "utils.h"
#include <iostream>

// updates the position based on the previous position
// return true is succeeded, false otherwise
bool Robot::update(std::vector<UndefinedCVObject>& cvObjs)
{
    if (cvObjs.size() < 4) return false;
    UndefinedCVObject newRear;
    UndefinedCVObject newFront;
    for (int i = 0; i < cvObjs.size(); i+=2) {
        if (distance(cvObjs.at(i).center, newRear.center) > 100.0) continue;

        if (newRear.area <= cvObjs.at(i).area) {
           newFront = newRear;
           newRear = cvObjs.at(i);
        } else if (newFront.area <= cvObjs.at(i).area) {
           newFront = cvObjs.at(i);
        }
    }
    if (newFront.area == 0 || newRear.area == 0) return false;
    frontDelay = front;
    rearDelay = rear;
    front = newFront;
    rear = newRear;
    theta = angleFrom(rear.center, front.center);
    return true;
}

// if we do not have a last location we can try to find a robot anywhere on the field
bool Robot::find(std::vector<cv::Moments> &moments)
{
    std::vector<UndefinedCVObject> cvObjs;
    for (int i = 0; i < moments.size(); i+=2) {
        UndefinedCVObject obj(moments[i]);
        cvObjs.push_back(obj);
    }

    if (cvObjs.size() < 2) return false;

    UndefinedCVObject newRear;
    UndefinedCVObject newFront;
    for (int i = 0; i < cvObjs.size(); i++) {
        if (newRear.area <= cvObjs.at(i).area) {
           newFront = newRear;
           newRear = cvObjs.at(i);
        } else if (newFront.area <= cvObjs.at(i).area) {
           newFront = cvObjs.at(i);
        }
    }
    if (newFront.area == 0 || newRear.area == 0) return false;
    frontDelay = front;
    rearDelay = rear;
    front = newFront;
    rear = newRear;
    theta = angleFrom(rear.center, front.center);
    return true;
}
