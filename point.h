#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
    friend std::ostream &operator<<(std::ostream&, Point);
    public:
        double _a, _b, _c;
        Point ();
        Point (double, double, double);
};

#endif /* POINT_H */
