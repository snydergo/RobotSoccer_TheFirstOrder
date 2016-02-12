#pragma once

#include "undefinedcvobject.h"
#include "definedcvobject.h"

class Robot: public DefinedCVObject
{
public:
  
  bool update(std::vector<UndefinedCVObject>& cvObjs) override;
  bool find(std::vector<UndefinedCVObject>& cvObjs) override;
  
private:
  UndefinedCVObject front;
  UndefinedCVObject rear;
  
  cv::Point2f center;

  float theta;
};
