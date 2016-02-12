
#include "ball.h"

bool Ball::update(std::vector<UndefinedCVObject> &cvObjs) {// find the closest ojbect within 50 cm
    if (cvObjs.empty()) return false;

    int closestIndex = findClosest(lastLocation, cvObjs);
    if (distance(lastLocation.center, cvObjs[closestIndex].center) < 50.0) {
        lastLocation = cvObjs[closestIndex];
        cvObjs.erase(cvObjs.begin() + closestIndex);
        return true;
    }

    return false;
}
bool Ball::find(std::vector<UndefinedCVObject>& cvObjs) { // find the largest object on the field
    if (cvObjs.empty()) return false;

    int largestIndex = findLargest(cvObjs);
    lastLocation = cvObjs[largestIndex];
    cvObjs.erase(cvObjs.begin() + largestIndex);


    return true;
}
