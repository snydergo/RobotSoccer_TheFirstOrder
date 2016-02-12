#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <string>

#include "point.h"

class FieldObject{
public:
  std::string tag;
  Point location;
  Point velocity;

  //FieldObject Functions
  FieldObject(std::string tag, Point loc, Point vel)
  : tag(tag)
  , velocity(vel)
  , location(loc)
  {
  }

  std::string toString();
};

#endif // FIELDOBJECT_H
