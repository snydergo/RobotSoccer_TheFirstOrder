
#include "robot.h"
#include "utils.h"

// updates the position based on the previous position
// return true is succeeded, false otherwise
bool Robot::update(std::vector<UndefinedCVObject>& cvObjs)
{
    if (cvObjs.empty()) return false;

    // Find the a object that could be the rear half of the jersey
    int rearIdx = -1;
    float deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(rear.center, cvObjs[i].center);
        if (std::abs(cvObjs[i].area - rearRefSize) < rearRefSizeDelta && // we found a point within our size threshold
            newDistance < 20.0  && // it is within our threshold distance
            newDistance < deltaDistance) // it is closer then the previous point
        {
            rearIdx = i;
            deltaDistance = newDistance;
        }
    }
    if (rearIdx == -1) return false;

    UndefinedCVObject potentialRear = cvObjs[rearIdx]; // This could be a rear half on a jersey

    // find an object that could be the front half of the jersey we found in the last step
    std::vector<int> frontIdxs;
    deltaDistance = 500.0;
    for(int i = 0; i < cvObjs.size(); i++) {
        float newDistance = distance(potentialRear.center, cvObjs[i].center);
        if (std::abs(refDistance - newDistance) < refDistance/2 && // we found a point within our distance threshold
            std::abs(cvObjs[i].area - frontRefSize) < frontRefSizeDelta) // it is within our threshold size
        {
            frontIdxs.push_back(i);
            deltaDistance = newDistance;
        }
    }
    
    // If there are multiple potential front halves of the jerseys find the one closest to the last known theta
    int frontIdx = -1;
    float deltaAngle = 400.0;
    for(int i = 0; i < frontIdxs.size(); i++) {
        int idx = frontIdxs[i];
        float angle = std::abs(angleDifference(theta, angleFrom(potentialRear.center, cvObjs[idx].center)));
        if (angle < deltaAngle) {
            frontIdx = idx;
            deltaAngle = angle;
        }
    }

    if (frontIdx == -1) return false;

    rear = potentialRear;
    front = cvObjs[frontIdx];

    // remove the objects we are using
    if (frontIdx < rearIdx) {
        cvObjs.erase(cvObjs.begin() + rearIdx);
        cvObjs.erase(cvObjs.begin() + frontIdx);

    } else {
        cvObjs.erase(cvObjs.begin() + frontIdx);
        cvObjs.erase(cvObjs.begin() + rearIdx);
    }
    
    // update theta
    theta = angleFrom(rear.center, front.center);
    return true;
}

// if we do not have a last location we can try to find a robot anywhere on the field
bool Robot::find(std::vector<UndefinedCVObject>& cvObjs) 
{
    if (cvObjs.empty()) return false;

    int rearIdx = -1;
    int size = 10000;
    for(int i = 0; i < cvObjs.size(); i++) {
        if (std::abs(cvObjs[i].area - rearRefSize) < rearRefSizeDelta && // found a point within our size threshold
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
            std::abs(cvObjs[i].area - frontRefSize) < frontRefSizeDelta) // it is within our threshold size
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

std::vector<Robot> findRobots(const std::vector<UndefinedCVObject>& cvObjs)
{
    if (cvObjs.size() < 2) return std::vector<Robot>();

    float minArea = 10000;
    float maxArea = 0;
    for (auto &i: cvObjs) {
        if (i.area < minArea) {
            minArea = i.area;
        }
        if (i.area > maxArea) {
            maxArea = i.area;
        }
    }
    float centerArea = (maxArea + minArea)/2;
    std::vector<UndefinedCVObject> smallObj;
    std::vector<UndefinedCVObject> largeObj;

    for (auto &i: cvObjs) {
        if (i.area < centerArea) {
            smallObj.push_back(i);
        }
        else {
            largeObj.push_back(i);
        }
    }
    int totalSize = 100000;
    std::vector<int> mapping;
    for (int l = 0; l < largeObj.size(); l++) {
        int size = 0;
        for (int s = 0; s < smallObj.size(); s++) {
            size += distance(largeObj[l].center, smallObj[s].center);
        }
    }

    return std::vector<Robot>();
}
