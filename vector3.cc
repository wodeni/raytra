#include "vector3.h"

Vector3::Vector3() {
    _a = _b = _c = 0.0;
}

Vector3::Vector3(double a, double b, double c) {
    _a = a; _b = b; _c = c;
}

Vector3 Vector3::crossproduct(Vector3 rhs) {
    double a = _b * rhs._c - _c * rhs._b; 
    double b = _c * rhs._a - _a * rhs._c;
    double c = _a * rhs._b - _b * rhs._a;
    Vector3 v(a, b, c);
    return v;
}

std::ostream &operator<<(std::ostream& os, const Vector3 v) {
    os << "<" << v._a << ", " << v._b << ", " << v._c << ">";      
    return os;
}

Vector3 operator*(double i, Vector3 v) {
    Vector3 w(i*v._a, i*v._b, i*v._c);
    return w;
}

Vector3 operator*(Vector3 v, double i) {
    Vector3 w(i*v._a, i*v._b, i*v._c);
    return w;
}

