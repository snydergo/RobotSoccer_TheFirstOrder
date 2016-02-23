#pragma once

#include "undefinedcvobject.h"
#include "definedcvobject.h"

class Robot: public DefinedCVObject
{
public:

  Robot(): simpleDetection(true){}

  Robot(int frontSize, int rearSize, int distance)
  : frontRefSize(frontSize)
  , rearRefSize(rearSize)
  , refDistance(distance)
  , simpleDetection(false)
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

  int frontRefSize;
  int rearRefSize;
  int refDistance;
  bool simpleDetection;

  float theta;
};
