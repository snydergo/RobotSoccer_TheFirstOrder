#ifndef UNDEFINEDCVOBJECT_H
#define UNDEFINEDCVOBJECT_H

#include "utils.h"
#include "hsvcolorsubspace.h"

class UndefinedCVObject
{
public:
    UndefinedCVObject();
    Coord center;
    int area;
    Color color;
};

#endif // UNDEFINEDCVOBJECT_H
