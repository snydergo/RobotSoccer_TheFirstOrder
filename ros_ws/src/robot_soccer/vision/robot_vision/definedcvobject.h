#pragma once

#include "undefinedcvobject.h"
#include "utils.h"

class DefinedCVObject
{
public:
  virtual bool update(std::vector<UndefinedCVObject>& cvObjs){return false;}
  virtual bool find(std::vector<UndefinedCVObject>& cvObjs){return false;}

protected:


  int findClosestSize(const int refSize, const std::vector<UndefinedCVObject>& cvObjs)
  {
      int index;
      int size = 10000;
      for(int i=0; i < cvObjs.size(); i++)
          if (std::abs(refSize - cvObjs[i].area) < size) {
              index = i;
              size = cvObjs[i].area;
          }
      return index;
  }

  int findClosestDistance(const UndefinedCVObject& ref, const std::vector<UndefinedCVObject>& cvObjs)
  {
      int closest;
      float deltaDistance = 500.0;
      for(int i=0; i < cvObjs.size(); i++) {
          float newDistance = distance(ref.center, cvObjs[i].center);
          if (deltaDistance > newDistance) {
              closest = i;
              deltaDistance = newDistance;
          }
      }
      return closest;
  }
};
