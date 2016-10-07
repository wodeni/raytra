#include "surface.h"
#include <iostream>
#include <cmath>

/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
Intersection Sphere::intersect(Ray &r) {
    Intersection result;
    double desc; // the descriminant of the solution
    Vector3 o_diff = r._origin - _origin; // p0 - O
    double d_sq = r._dir.dotproduct(r._dir); // d * d
    double a = r._dir.dotproduct(o_diff); // d * (p0 - O) 
    double b = o_diff.dotproduct(o_diff) - (_radius * _radius); // (p0 - O)(p0 - O) - r^2 
    desc = a * a - d_sq * b; 
    if(desc < 0) {
        result.setIntersected(false);
        return result;
    }
    result.setIntersected(true);
    result.setTs((-1 * a + sqrt(desc)) / d_sq, 
                 (-1 * a - sqrt(desc)) / d_sq);
    return result;
}
