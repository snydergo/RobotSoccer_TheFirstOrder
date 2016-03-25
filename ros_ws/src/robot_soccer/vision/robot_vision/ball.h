#pragma once

#include "definedcvobject.h"
#include "undefinedcvobject.h"

class Ball
{ 
public:

    Ball(): thresholdDistance(45.0){}


    Ball(int _refSize)
    : refSize(_refSize)
    , thresholdDistance(45.0)
    {
    }

    bool update(std::vector<UndefinedCVObject>& cvObjs);
    bool find(std::vector<cv::Moments> moments);
    
    cv::Point2f getCenter() {return currentLocation.center;}

private:
    UndefinedCVObject currentLocation;
    UndefinedCVObject lastLocation;

    int refSize;
    float thresholdDistance;
};
