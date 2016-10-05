#include "point.h"

Point::Point() {
    _a = _b = _c = 0.0;
}

Point::Point(double a, double b, double c) {
    _a = a; _b = b; _c = c;
}


std::ostream &operator<<(std::ostream& os, Point p) {
    std::cout << "(" << p._a << ", " << p._b << ", " << p._c << ")";
    return os;
}
