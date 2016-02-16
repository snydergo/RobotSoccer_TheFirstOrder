#ifndef UTILS_H
#define UTILS_H

#include "cmath"

#define PI 3.14159265

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

template<template<typename> class P, typename T>
T distance(const P<T>& p1, const P<T>& p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

template<typename T, typename S>
T angleDifference(T currentTheta, S desiredTheta){
    double angle = currentTheta - desiredTheta;
    if (angle < -180) {
        angle = -(360 - std::abs(angle));
    } else if (angle > 180) {
        angle = 360 - angle;
    } else {
        angle = -angle;
    }
    return angle;
}

template<template<typename> class P, typename T>
T angleFrom(const P<T>& p1, const P<T>& p2)
{
    return std::atan2(p2.y - p1.y, p2.x - p1.x) * 180 / PI;
}



#endif // UTILS_H
