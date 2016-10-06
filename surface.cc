#include "surface.h"
#include <cmath>
Sphere::Sphere(Point origin, float radius)
    : _origin(origin), _radius(radius)
{}


std::ostream &operator<<(std::ostream& os, Sphere& s) {
    os << s._origin <<  " " << s._radius;
    return os;
}

Intersection Sphere::intersect(Ray r) {
    Intersection result;
    double desc; // the descriminant of the solution
    Vector3 o_diff = r._origin - _origin; // p0 - O
    double d_sq = r._dir.dotproduct(r._dir); // d * d
    double a = r._dir.dotproduct(o_diff); // d * (p0 - O) 
    double b = o_diff.dotproduct(o_diff) - (_radius * _radius); // (p0 - O)(p0 - O) - r^2 
    desc = a * a - d_sq * b; 
    if(desc < 0) {
        result.intersected = 0;
        return result;
    }
    result.intersected = 1;
    result._t1 = (-1 * a + sqrt(desc)) / d_sq;
    result._t2 = (-1 * a - sqrt(desc)) / d_sq;
}
