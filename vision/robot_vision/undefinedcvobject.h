#ifndef UNDEFINEDCVOBJECT_H
#define UNDEFINEDCVOBJECT_H

#include "utils.h"
#include "hsvcolorsubspace.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "hsvcolorsubspace.h"

using cv::Point2f;
using cv::Moments;

class UndefinedCVObject
{
public:
    UndefinedCVObject(){}
    UndefinedCVObject(Moments moment, HsvColorSubSpace _color);
    Point2f center;
    int area;
    Color color;
};

#endif // UNDEFINEDCVOBJECT_H
