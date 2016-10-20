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
    // - (N dot P0 - d)
    // Doing origin - (0,0,0) because we need to convert from a
    // point to a vector
    Vector3 origin = r._origin - Point(0,0,0);
    double upper_term = -1 * (_normal.dotproduct(origin) - _d);
    double t = upper_term / dotproduct;

    // It is a valid intersection only when t is positive
    if(t > 0) {
        result.setIntersected(true);
        result.setT(t);
        Point intersectionPoint = r._origin + (t * r._dir);
        result.setIntersectionPoint(intersectionPoint);
        result.setNormal(_normal);
    } else {
        result.setIntersected(false);
    }
    return result;
}

Intersection Triangle::intersect(Ray& ray) {

	Intersection res;

	double t0 = 0.0;
	double t1 = 1000000.0;

	// Using the notation on the textbook. E.g: Vector abc has componets <a, b, c>
	Vector3 abc = _p1 - _p2; // a - b
	Vector3 def = _p1 - _p3; // a - c
	Vector3 ghi = ray._dir; // d
	Vector3 jkl = _p1 - ray._origin; // a - e
	double M, t, beta, gamma;
	// Symbolizing the entries in matrix A
	double a = abc._a, b = abc._b, c = abc._c;
	double d = def._a, e = def._b, f = def._c;
	double g = ghi._a, h = ghi._b, i = ghi._c;
	double j = jkl._a, k = jkl._b, l = jkl._c;

	double ak_jb = a * k - j * b; // ak - jb
	double jc_al = j * c - a * l; // jc - al
	double bl_kc = b * l - k * c; // bl - kc
	double ei_hf = e * i - h * f; // ei - hf
	double gf_di = g * f - d * i; // gf - di
	double dh_eg = d * h - e * g; // dh - eg

	M = a * ei_hf + b * gf_di + c * dh_eg; // |A|
	t = -1.0 * (f * ak_jb + e * jc_al + d * bl_kc) / M; // Compute the value of t
	if (t < t0 or t > t1) {
		res.setIntersected(false);
		return res;
	}
	gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M; // Compute the value of gamma
	if(gamma < 0 or gamma > 1) {
		res.setIntersected(false);
		return res;
	}
	beta = (j * ei_hf + k * gf_di + l * dh_eg) / M; // Compute the value of beta
	if(beta < 0 or beta > (1 - gamma)) {
		res.setIntersected(false);
		return res;
	}

	res.setIntersected(true);
	res.setT(t);
	// Normal = (b - a) x (c - a)
	Vector3 u = -1. * abc;
	Vector3 v = -1. * def;
	u.normalize(); v.normalize();
	Vector3 N = u.crossproduct(v);
	N.normalize();
	res.setNormal(N);
    Point intersectionPoint = ray._origin + (t * ray._dir);
    res.setIntersectionPoint(intersectionPoint);
    return res;
}
