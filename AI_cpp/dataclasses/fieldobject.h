#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <string>

#include "point.h"
#include "../types.h"

class FieldObject{
public:
  robotType tag;
  Point location;
  Point velocity;

  //FieldObject Functions
  FieldObject(robotType tag, Point loc, Point vel)
  : tag(tag)
  , velocity(vel)
  , location(loc)
  {
  }

  std::string toString();
};

#endif // FIELDOBJECT_H
