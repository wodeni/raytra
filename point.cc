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

Vector3 Point::operator-(Point p2) {
    Vector3 v(_a-p2._a, _b-p2._b, _c-p2._c);
    return v;
}
