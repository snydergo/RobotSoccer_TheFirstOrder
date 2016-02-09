#pragma once

#include "definedcvobject.h"
#include "undefinedcvobject.h"

public Ball: public DefinedCVObject
{ 
public:
    bool update(std::vector<UndefinedCVObject> cvObjs) override;
    bool find(std::vector<UndefinedCVObject> cvObjs) override;
    
    UndefinedCVObject lastLocation;
    
    cv::Point2f center;
};
