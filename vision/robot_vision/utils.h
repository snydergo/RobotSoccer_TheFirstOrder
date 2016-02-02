#ifndef UTILS_H
#define UTILS_H

struct Range {
    int low;
    int high;
};

const Range orangeHue{0,22};
const Range yellowHue{22,38};
const Range greenHue{38,75};
const Range blueHue{75,130};
const Range violetHue{130,160};
const Range redHue{160,179};

enum class Color {
    orange,
    yellow,
    green,
    blue,
    violet,
    red,
    undefined
};

Color ConvertHueRangeToColor(Range hueRange);

#endif // UTILS_H
