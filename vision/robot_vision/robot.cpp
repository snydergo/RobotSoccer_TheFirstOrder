
#include "robot.h"
#include "utils.h"

bool Robot::update(std::vector<UndefinedCVObject>& cvObjs)
{
    if (cvObjs.empty()) return false;

    int rearIdx = -1;
    float deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(rear.center, cvObjs[i].center);
        if (std::abs(cvObjs[i].area - rearRefSize) < rearRefSize/2 && // we found a point within our size threshold
            newDistance < 20.0  && // it is within our threshold distance
            newDistance < deltaDistance) // it is closer then the previous point
        {
            rearIdx = i;
            deltaDistance = newDistance;
        }
    }
    if (rearIdx == -1) return false;

    UndefinedCVObject potentialRear = cvObjs[rearIdx];

    std::vector<int> frontIdxs;
    deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(potentialRear.center, cvObjs[i].center);
        if (std::abs(refDistance - newDistance) < refDistance/2 && // we found a point within our distance threshold
            std::abs(cvObjs[i].area - frontRefSize) < frontRefSize/2) // it is within our threshold size
        {
            frontIdxs.push_back(i);
            deltaDistance = newDistance;
        }
    }
    int frontIdx = -1;
    float deltaAngle = 400.0;
    for(int i = 0; i < frontIdxs.size(); i++) {
        int idx = frontIdxs[i];
        float angle = std::abs(angleDifference(theta, angleFrom(potentialRear.center, cvObjs[idx].center)));
        if (angle < deltaAngle) {
            frontIdx = i;
            deltaAngle = angle;
        }
    }

    if (frontIdx == -1) return false;

    rear = potentialRear;
    front = cvObjs[frontIdx];

    if (frontIdx < rearIdx) {
        cvObjs.erase(cvObjs.begin() + rearIdx);
        cvObjs.erase(cvObjs.begin() + frontIdx);

    } else {
        cvObjs.erase(cvObjs.begin() + frontIdx);
        cvObjs.erase(cvObjs.begin() + rearIdx);
    }
    theta = angleFrom(rear.center, front.center);
    return true;
}

bool Robot::find(std::vector<UndefinedCVObject>& cvObjs) // find the largest object and then one closet to it
{
    if (cvObjs.empty()) return false;

    int rearIdx = -1;
    int size = 10000;
    for(int i = 0; i < cvObjs.size(); i++) {
        if (std::abs(cvObjs[i].area - rearRefSize) < rearRefSize/2 && // found a point within our size threshold
            std::abs(rearRefSize - cvObjs[i].area) < size) // it is closer in size then the previous point
        {
            rearIdx = i;
            size = cvObjs[i].area;
        }
    }
    if (rearIdx == -1) return false;

    UndefinedCVObject potentialRear = cvObjs[rearIdx];

    int frontIdx = -1;
    float deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(potentialRear.center, cvObjs[i].center);
        if (std::abs(refDistance - newDistance) < refDistance/2 && // we found a point within our distance threshold
            std::abs(refDistance - newDistance) < deltaDistance && // it is closer then the previous point
            std::abs(cvObjs[i].area - frontRefSize) < frontRefSize/2) // it is within our threshold size
        {
            frontIdx = i;
            deltaDistance = newDistance;
        }
    }

    if (frontIdx == -1) return false;

    rear = potentialRear;
    front = cvObjs[frontIdx];

    if (frontIdx < rearIdx) {
        cvObjs.erase(cvObjs.begin() + rearIdx);
        cvObjs.erase(cvObjs.begin() + frontIdx);

    } else {
        cvObjs.erase(cvObjs.begin() + frontIdx);
        cvObjs.erase(cvObjs.begin() + rearIdx);
    }
    theta = angleFrom(rear.center, front.center);
    return true;

}
