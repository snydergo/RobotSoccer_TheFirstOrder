#pragma once

#include "definedcvobject.h"
#include "undefinedcvobject.h"

class Ball: public DefinedCVObject
{ 
public:

    Ball(): thresholdDistance(45.0){}


    Ball(int _refSize)
    : refSize(_refSize)
    , thresholdDistance(45.0)
    {
    }

    bool update(std::vector<UndefinedCVObject>& cvObjs) override;
    bool find(std::vector<UndefinedCVObject>& cvObjs) override;
    
    cv::Point2f getCenter() {return lastLocation.center;}

private:
    UndefinedCVObject lastLocation;
    int refSize;
    float thresholdDistance;
};
