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
    UndefinedCVObject(): area(0){}

    UndefinedCVObject(Moments moment);

    Point2f center;
    int area = 0;
};

#endif // UNDEFINEDCVOBJECT_H
