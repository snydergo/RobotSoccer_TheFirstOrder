#pragma once

#include "undefinedcvobject.h"

class Robot
{
public:
  
  bool update(std::vector<UndefinedCVObject> cvObjs);
  bool find(std::vector<UndefinedCVObject> cvObjs);
  
private:
  UndefinedCVObject front;
  UndefinedCVObject rear;
  
  cv::Point2f center;
  float theta;

};
