#include "undefinedcvobject.h"
#include "utils.h"
#include "objectdetection.h"

using namespace cv;

UndefinedCVObject::UndefinedCVObject() {}

UndefinedCVObject::UndefinedCVObject(Moments moment, HsvColorSubSpace _color)
{
    center = GetMomentCenter(moment);
    area = GetMomentArea(moment);
    color = ConvertHueRangeToColor(_color.hue);
}
