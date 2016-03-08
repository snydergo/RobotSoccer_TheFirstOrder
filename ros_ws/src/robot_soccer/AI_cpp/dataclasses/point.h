/* Simple Data structure that contains just and x and y coordinate*/
#ifndef POINT_H
#define POINT_H

#include <string>



class Point
{
public:
    double x;
    double y;
    //Point Functions
    Point(): x(0), y(0) {}
    Point(double x, double y): x(x), y(y) {}
    std::string toString();
};

#endif // POINT_H
