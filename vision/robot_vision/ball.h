#pragma once

#include "definedcvobject.h"
#include "undefinedcvobject.h"

class Ball: public DefinedCVObject
{ 
public:

    Ball(int _refSize)
    : refSize(_refSize)
    , thresholdDistance(45.0)
    {
    }

    bool update(std::vector<UndefinedCVObject>& cvObjs) override;
    bool find(std::vector<UndefinedCVObject>& cvObjs) override;
    
    cv::Point2f getCenter() {return center;}

private:
    UndefinedCVObject lastLocation;
    cv::Point2f center;
    int refSize;
    float thresholdDistance;
};
