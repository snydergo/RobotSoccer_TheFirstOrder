#pragma once

#include "undefinedcvobject.h"

class DefinedCVObject
{
public:
  virtual bool update(std::vector<UndefinedCVObject> cvObjs){return false;}
  virtual bool find(std::vector<UndefinedCVObject> cvObjs){return false;}
  
  cv::Point2f center;
};
