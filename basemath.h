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
        double length() const;
        void normalize();
        Vector3 operator+(const Vector3);
        void operator+=(const Vector3);
        friend Vector3 operator*(double, Vector3);
        friend Vector3 operator*(Vector3, double);
        friend std::ostream &operator<<(std::ostream&, Vector3);
};

class Point {
    friend std::ostream &operator<<(std::ostream&, Point);
    public:
        double _a, _b, _c;
        Point ();
        Point (double, double, double);
        Vector3 operator-(Point);
        friend Point operator+(Point, Vector3);
        friend Point operator+(Vector3, Point);
};


#endif /* BASEMATH_H */
