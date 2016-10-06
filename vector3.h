#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Vector3 {
    public:
        double _a, _b, _c;
        Vector3 ();
        Vector3 (double, double, double);
        Vector3 crossproduct (Vector3 &);
        double dotproduct (Vector3 &);
        Vector3 normalize();
        Vector3 operator+(Vector3);
        friend Vector3 operator*(double, Vector3);
        friend Vector3 operator*(Vector3, double);
        friend std::ostream &operator<<(std::ostream&, Vector3);
};

#endif /* VECTOR3_H */
