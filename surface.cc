#include "surface.h"
#include <iostream>
#include <cmath>

/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
Intersection Sphere::intersect(Ray &r) {
    Intersection result;
    double disc; // the discriminant of the solution
    Vector3 o_diff = r._origin - _origin; // p0 - O
    double d_sq = r._dir.dotproduct(r._dir); // d * d
    double a = r._dir.dotproduct(o_diff); // d * (p0 - O) 
    double b = o_diff.dotproduct(o_diff) - (_radius * _radius); // (p0 - O)(p0 - O) - r^2 
    disc = a * a - d_sq * b;

    if(disc < 0.) {
        result.setIntersected(false);
        return result;
    }

    double desc_sqrt = sqrt(disc);
    double t = (-1.0 * a + desc_sqrt) / d_sq;

    if(disc > 0.) {
    	double temp_t = (-1.0 * a - desc_sqrt) / d_sq;
    	if(temp_t > 0 and (t <= 0 or temp_t < t))
    		t = temp_t;
    }

    if(t <= 0) {
    	result.setIntersected(false);
    	return result;
    }
    result.setIntersected(true);
    // Store the parametized t values
    result.setT(t);
    // Compute the closest intersection point using the smaller t
    Point intersectionPoint = r._origin + (t * r._dir);
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

    // It is a valid intersection only when t is positive
    if(t > 0) {
        result.setIntersected(true);
        result.setT(t);
        Point intersectionPoint = r._origin + (result.getT() * r._dir);
        result.setIntersectionPoint(intersectionPoint);
        result.setNormal(_normal);
    } else {
        result.setIntersected(false);
    }
    return result;
}
