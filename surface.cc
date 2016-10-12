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
    double desc_sqrt = sqrt(desc);
    // Store the parametized t values
    result.setTs((-1 * a + desc_sqrt) / d_sq, 
                 (-1 * a - desc_sqrt) / d_sq);
    // Compute the closest intersection point using the smaller t
    Point intersectionPoint = r._origin + (result.getT() * r._dir);
    result.setIntersectionPoint(intersectionPoint);
    // Set the surface normal, which is (intersection - origin) / radius
    result.setNormal( (1 / _radius) * (intersectionPoint - _origin) );
    return result;
}

Intersection Plane::intersect(Ray & r) {
    Intersection result;
    // vec_d dot N 
    double dotproduct = r._dir.dotproduct(_normal);
    // Not intersection if the plane is parallel with the ray
    if(dotproduct == 0) {
        result.setIntersected(false);
        return result;
    }
    // - (d + N dot P0)
    // Doing origin - (0,0,0) because we need to convert from a
    // point to a vector
    Vector3 origin = r._origin - Point(0,0,0);
    double upper_term = -1 * (_d + _normal.dotproduct(origin));
    double t = upper_term / dotproduct;
    if(t > 0) {
        result.setIntersected(true);
        result.setTs(t, t); // Although there is only one intersection, we
                            // still put t for both fields for consistency
        Point intersectionPoint = r._origin + (result.getT() * r._dir);
        result.setIntersectionPoint(intersectionPoint);
        result.setNormal(_normal);
    } else {
        result.setIntersected(false);
    }
    return result;
}
