#pragma once

#include "undefinedcvobject.h"
#include "definedcvobject.h"

class Robot: public DefinedCVObject
{
public:

  Robot()
  : front()
  , rear()
  {
  }

  bool update(std::vector<UndefinedCVObject>& cvObjs) override;
  bool find(std::vector<UndefinedCVObject>& cvObjs) override;

  cv::Point2f getCenter()
  {
      return cv::Point2f((front.center.x + rear.center.x)/2,
                         (front.center.y + rear.center.y)/2);
  }
  float getRotation()
  {
      return theta;
  }
  
private:
  UndefinedCVObject front;
  UndefinedCVObject rear;

  UndefinedCVObject frontDelay;
  UndefinedCVObject rearDelay;

  float theta;
};
