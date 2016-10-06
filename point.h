#ifndef POINT_H
#define POINT_H

#include <iostream>
#include "vector3.h"

class Point {
    friend std::ostream &operator<<(std::ostream&, Point);
    public:
        double _a, _b, _c;
        Point ();
        Point (double, double, double);
        Vector3 operator-(Point);
};

#endif /* POINT_H */
