#include "basemath.h"
#include <cmath>
#include <cassert>

Vector3::Vector3() {
    _a = _b = _c = 0.0;
}

Vector3::Vector3(double a, double b, double c) {
    _a = a; _b = b; _c = c;
}

Vector3 Vector3::crossproduct(const Vector3 &rhs) const {
    double a = _b * rhs._c - _c * rhs._b; 
    double b = _c * rhs._a - _a * rhs._c;
    double c = _a * rhs._b - _b * rhs._a;
    Vector3 v(a, b, c);
    return v;
}

void Vector3::normalize() {
    assert( _a != 0 || _b != 0 || _c != 0);
    double a2 = _a * _a;
    double b2 = _b * _b;
    double c2 = _c * _c;
    double len =  sqrt(a2 + b2 + c2);
    _a /= len; _b /= len; _c /= len;
}

double Vector3::dotproduct(const Vector3 &v) const {
    return _a * v._a + _b * v._b + _c * v._c;
}

double Vector3::length() const {
    double a2 = _a * _a;
    double b2 = _b * _b;
    double c2 = _c * _c;
    return sqrt(a2 + b2 + c2);
}

Vector3 Vector3::operator+(const Vector3 &rhs) const {
    Vector3 res(_a + rhs._a, _b + rhs._b, _c + rhs._c);
    return res;
}

Vector3 Vector3::operator-(const Vector3 &rhs) const {
    Vector3 res(_a - rhs._a, _b - rhs._b, _c - rhs._c);
    return res;
}

Vector3 Vector3::pieceMultiply(const Vector3& rhs) const {
	return Vector3 (_a * rhs._a, _b * rhs._b, _c * rhs._c);
}

std::ostream &operator<<(std::ostream& os, const Vector3 v) {
    os << "<" << v._a << ", " << v._b << ", " << v._c << ">";      
    return os;
}

Vector3 operator*(double i, const Vector3 &v) {
    Vector3 w(i*v._a, i*v._b, i*v._c);
    return w;
}

Vector3 operator*(const Vector3 &v, double i) {
    Vector3 w(i*v._a, i*v._b, i*v._c);
    return w;
}

double Vector3::operator [](int i) const {
	switch(i) {
	case 0: return _a;
	case 1: return _b;
	case 2: return _c;
	default: return 0; // TODO: what should be the default action here?
	}
}

void Vector3::operator+=(const Vector3 &rhs) {
    _a += rhs._a;
    _b += rhs._b;
    _c += rhs._c;
}

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

Vector3 Point::operator-(const Point &p2) const {
    Vector3 v(_a-p2._a, _b-p2._b, _c-p2._c);
    return v;
}

double Point::operator [](int i) const {
	switch(i) {
	case 0: return _a;
	case 1: return _b;
	case 2: return _c;
	default: return 0; // TODO: what should be the default action here?
	}
}

Point operator+(const Point &p, const Vector3 &v) {
    return Point(p._a + v._a, p._b + v._b, p._c + v._c);
}

Point operator+(const Vector3 &v, const Point &p) {
    return Point(p._a + v._a, p._b + v._b, p._c + v._c);
}


