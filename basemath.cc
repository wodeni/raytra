#include "basemath.h"
#include <cmath>
#include <cassert>

Vector3::Vector3() {
//    _xyz[0] = _xyz[1] = _xyz[2] = 0.0;
}

Vector3::Vector3(double a, double b, double c) {
    _xyz[0] = a; _xyz[1] = b; _xyz[2] = c;
}

Vector3 Vector3::crossproduct(const Vector3 &rhs) const {
    double a = _xyz[1] * rhs._xyz[2] - _xyz[2] * rhs._xyz[1];
    double b = _xyz[2] * rhs._xyz[0] - _xyz[0] * rhs._xyz[2];
    double c = _xyz[0] * rhs._xyz[1] - _xyz[1] * rhs._xyz[0];
    Vector3 v(a, b, c);
    return v;
}

void Vector3::normalize() {
    assert( _xyz[0] != 0 || _xyz[1] != 0 || _xyz[2] != 0);
    double a2 = _xyz[0] * _xyz[0];
    double b2 = _xyz[1] * _xyz[1];
    double c2 = _xyz[2] * _xyz[2];
    double len =  sqrt(a2 + b2 + c2);
    _xyz[0] /= len; _xyz[1] /= len; _xyz[2] /= len;
}

double Vector3::dotproduct(const Vector3 &v) const {
    return _xyz[0] * v._xyz[0] + _xyz[1] * v._xyz[1] + _xyz[2] * v._xyz[2];
}

double Vector3::length() const {
    double a2 = _xyz[0] * _xyz[0];
    double b2 = _xyz[1] * _xyz[1];
    double c2 = _xyz[2] * _xyz[2];
    return sqrt(a2 + b2 + c2);
}

Vector3 Vector3::operator+(const Vector3 &rhs) const {
    Vector3 res(_xyz[0] + rhs._xyz[0], _xyz[1] + rhs._xyz[1], _xyz[2] + rhs._xyz[2]);
    return res;
}

Vector3 Vector3::operator-(const Vector3 &rhs) const {
    Vector3 res(_xyz[0] - rhs._xyz[0], _xyz[1] - rhs._xyz[1], _xyz[2] - rhs._xyz[2]);
    return res;
}

Vector3 Vector3::pieceMultiply(const Vector3& rhs) const {
	return Vector3 (_xyz[0] * rhs._xyz[0], _xyz[1] * rhs._xyz[1], _xyz[2] * rhs._xyz[2]);
}

std::ostream &operator<<(std::ostream& os, const Vector3 v) {
    os << "<" << v._xyz[0] << ", " << v._xyz[1] << ", " << v._xyz[2] << ">";
    return os;
}

Vector3 operator*(double i, const Vector3 &v) {
    Vector3 w(i*v._xyz[0], i*v._xyz[1], i*v._xyz[2]);
    return w;
}

Vector3 operator*(const Vector3 &v, double i) {
    Vector3 w(i*v._xyz[0], i*v._xyz[1], i*v._xyz[2]);
    return w;
}

double Vector3::operator [](int i) const {
	return _xyz[i];
}

void Vector3::operator+=(const Vector3 &rhs) {
    _xyz[0] += rhs._xyz[0];
    _xyz[1] += rhs._xyz[1];
    _xyz[2] += rhs._xyz[2];
}

Point::Point() {
//    _xyz[0] = _xyz[1] = _xyz[2] = 0.0;
}

Point::Point(double a, double b, double c) {
    _xyz[0] = a; _xyz[1] = b; _xyz[2] = c;
}

std::ostream &operator<<(std::ostream& os, Point p) {
    std::cout << "(" << p._xyz[0] << ", " << p._xyz[1] << ", " << p._xyz[2] << ")";
    return os;
}

Vector3 Point::operator-(const Point &p2) const {
    Vector3 v(_xyz[0]-p2._xyz[0], _xyz[1]-p2._xyz[1], _xyz[2]-p2._xyz[2]);
    return v;
}

double Point::operator [](int i) const {
	return _xyz[i];
}

Point operator+(const Point &p, const Vector3 &v) {
    return Point(p._xyz[0] + v._xyz[0], p._xyz[1] + v._xyz[1], p._xyz[2] + v._xyz[2]);
}

Point operator+(const Vector3 &v, const Point &p) {
    return Point(p._xyz[0] + v._xyz[0], p._xyz[1] + v._xyz[1], p._xyz[2] + v._xyz[2]);
}

Vector3::operator Point() {
	return Point(_xyz[0], _xyz[1], _xyz[2]);
}



