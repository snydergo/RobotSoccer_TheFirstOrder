#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <string>
#include <cstddef>

#include "point.h"

class FieldObject{
public:
  std::string tag = "";
  Point location;
  Point velocity;
  FieldObject(){}
  //FieldObject Functions
  FieldObject(std::string tag, Point loc, Point vel);
  std::string toString();
};

#endif // FIELDOBJECT_H
