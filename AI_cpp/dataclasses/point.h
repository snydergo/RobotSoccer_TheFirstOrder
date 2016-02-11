#ifndef POINT_H
#define POINT_H

#include <string>
#include <cstddef>

enum robotType {ally1_rt, ally2_rt, enemy1_rt, enemy2_rt, none_rt};

class Point
{
public:
    double x = 0;
    double y = 0;
    //Point Functions
    Point();
    Point(double x, double y);
    std::string toString();
};

#endif // POINT_H
