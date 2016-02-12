#pragma once

#include "undefinedcvobject.h"
#include "utils.h"

class DefinedCVObject
{
public:
  virtual bool update(std::vector<UndefinedCVObject>& cvObjs){return false;}
  virtual bool find(std::vector<UndefinedCVObject>& cvObjs){return false;}

protected:
    int findLargest(const std::vector<UndefinedCVObject>& cvObjs)
    {
        int index;
        int largest;
        for(int i=0; i < cvObjs.size(); i++)
            if (largest < cvObjs[i].area) {
                index = i;
                largest = cvObjs[i].area;
            }
        return index;
    }

    int findClosest(const UndefinedCVObject& ref, const std::vector<UndefinedCVObject>& cvObjs)
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
