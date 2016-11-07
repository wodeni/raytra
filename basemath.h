/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef BASEMATH_H
#define BASEMATH_H

#include <iostream>

class Vector3 {
    public:
        double _a, _b, _c;
        Vector3 ();
        Vector3 (double, double, double);
        Vector3 crossproduct (const Vector3 &) const;
        double dotproduct (const Vector3 &) const;
        Vector3 pieceMultiply(const Vector3 &) const;
        double length() const;
        void normalize();
        Vector3 operator+(const Vector3&) const;
        Vector3 operator-(const Vector3&) const;
        void operator+=(const Vector3&);
        double operator[](int) const;

        friend Vector3 operator*(double, const Vector3&);
        friend Vector3 operator*(const Vector3&, double);

        friend std::ostream &operator<<(std::ostream&, Vector3);
};

class Point {
    friend std::ostream &operator<<(std::ostream&, Point);
    public:
        double _a, _b, _c;
        Point ();
        Point (double, double, double);
        Vector3 operator-(const Point&) const;
        friend Point operator+(const Point&, const Vector3&);
        friend Point operator+(const Vector3&, const Point&);
};


#endif /* BASEMATH_H */
