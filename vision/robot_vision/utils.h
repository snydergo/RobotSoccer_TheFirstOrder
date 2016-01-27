#ifndef UTILS_H
#define UTILS_H

struct Coord {
    int xPos;
    int yPos;
};

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

Color ConvertHueRangeToColor(Range hueRange)
{
    int hue = (hueRange.high + hueRange.low)/2;
    if (hue < orangeHue.low) return Color::undefined;

    if (hue < orangeHue.high) return Color::orange;
    if (hue < yellowHue.high) return Color::yellow;
    if (hue < greenHue.high ) return Color::green;
    if (hue < blueHue.high  ) return Color::blue;
    if (hue < violetHue.high) return Color::violet;
    if (hue < redHue.high   ) return Color::red;
    return Color::undefined;
}

#endif // UTILS_H
