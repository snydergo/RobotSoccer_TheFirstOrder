#pragma once

#include "utils.h"

// the color subspace we are going to detect
struct HsvColorSubSpace {

    HsvColorSubSpace(){}
    
    HsvColorSubSpace(Range hue, Range sat, Range value)
    : hue(hue)
    , saturation(sat)
    , value(value)
    {
    }
    Range hue; // 0 - 179 color
    Range saturation; // 0 - 255 deepness of color
    Range value; // 0 - 255 Intesity
};
