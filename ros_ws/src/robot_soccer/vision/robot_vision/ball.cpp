
#include "ball.h"

bool Ball::update(std::vector<UndefinedCVObject> &cvObjs) // find the closest ojbect within 45 cm
{
    if (cvObjs.empty()) return false;

    int closest = -1;
    float deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(currentLocation.center, cvObjs[i].center);
        if (newDistance < deltaDistance && // we found a new point
            newDistance < thresholdDistance && // it is within our threshold distance
            std::abs(cvObjs[closest].area - refSize) < refSize/2) // it is within our threshold size
        {
            closest = i;
            deltaDistance = newDistance;
        }
    }

    if (closest > -1) {
        lastLocation = currentLocation;
        currentLocation = cvObjs[closest];
        cvObjs.erase(cvObjs.begin() + closest);
        return true;
    }

    return false;
}
bool Ball::find(std::vector<UndefinedCVObject>& cvObjs) // find the object closest in size on the field
{
    if (cvObjs.empty()) return false;

    int closest = -1;
    int size = 10000;
    for(int i = 0; i < cvObjs.size(); i++) {
        int deltaSize = std::abs(refSize - cvObjs[i].area);
        if (deltaSize < size && // this object is closer in size then the old one
            deltaSize < refSize/2) // it is within our threshold distance
        {
            closest = i;
            size = deltaSize;
        }
    }

    if (closest > -1) {
        lastLocation = currentLocation;
        currentLocation = cvObjs[closest];
        cvObjs.erase(cvObjs.begin() + closest);
        return true;
    }

    return false;
}
