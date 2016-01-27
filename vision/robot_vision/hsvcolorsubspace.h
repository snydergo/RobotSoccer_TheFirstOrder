#include "utils.h"
// the color subspace we are going to detect
struct HsvColorSubSpace {


    Range hue; // 0 - 179 color
    Range saturation; // 0 - 255 deepness of color
    Range value; // 0 - 255 Intesity
};
