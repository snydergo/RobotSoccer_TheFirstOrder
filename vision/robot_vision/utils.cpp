
#include "utils.h"
#include "cmath"


Color ConvertHueRangeToColor(Range hueRange)
{
    int hue = (hueRange.high + hueRange.low)/2;
    if (hue < orangeHue.low ) return Color::undefined;
    if (hue < orangeHue.high) return Color::orange;
    if (hue < yellowHue.high) return Color::yellow;
    if (hue < greenHue.high ) return Color::green;
    if (hue < blueHue.high  ) return Color::blue;
    if (hue < violetHue.high) return Color::violet;
    if (hue < redHue.high   ) return Color::red;
    return Color::undefined;
}
