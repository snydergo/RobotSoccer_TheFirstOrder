#include "undefinedcvobject.h"
#include "utils.h"
#include "objectdetection.h"
#include "config.h"

using namespace cv;

UndefinedCVObject::UndefinedCVObject(Moments moment)
{
    Point2f point(GetMomentCenter(moment));
    center.x = (point.x - config::fieldCenter_px.x) * config::cmPerPixelConversionFactor;
    center.y = (point.y - config::fieldCenter_px.y) * config::cmPerPixelConversionFactor;
    if (config::invertX) {
        center.x = -center.x;
    } else {
        center.y = -center.y;
    }
    area = GetMomentArea(moment);
}
