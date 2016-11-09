/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#include "surface.h"
#include "constants.h"
#include <iostream>
#include <cmath>
#include <limits>


void Intersection::copy(const Intersection &c) {
	_t = c.getT();
	_normal = c.getNormal();
	_intersectionPoint = c.getIntersectionPoint();
	_intersected = c.Intersected();
}


bool Sphere::intersect(const Ray &r, Intersection &in) {
	if (!this->checkbox(r, in))
		return false;
	else {
		if (mode == BBOX_ONLY_MODE)
			return true;
		double disc; // the discriminant of the solution
		Vector3 o_diff = r._origin - _origin; // p0 - O
		double d_sq = r._dir.dotproduct(r._dir); // d * d
		double a = r._dir.dotproduct(o_diff); // d * (p0 - O)
		double b = o_diff.dotproduct(o_diff) - (_radius * _radius); // (p0 - O)(p0 - O) - r^2
		disc = a * a - d_sq * b;

		if (disc < 0.)
			return false;

		double desc_sqrt = sqrt(disc);
		double t = (-1.0 * a + desc_sqrt) / d_sq;

		if (disc > 0.) {
			double temp_t = (-1.0 * a - desc_sqrt) / d_sq;
			if (temp_t > 0 and (t <= 0 or temp_t < t))
				t = temp_t;
		}

		if (t <= 0)
			return false;
		Point pt = r._origin + (t * r._dir);
		Vector3 n = (1 / _radius) * (pt - _origin);
		in.set(t, pt, n);
		return true;
	}
}

bool Plane::intersect(const Ray &r, Intersection &in) {

	if (!this->checkbox(r, in))
		return false;

	// vec_d dot N
	double dotproduct = r._dir.dotproduct(_normal);
	// Not intersection if the plane is parallel with the ray
	if (dotproduct == 0)
		return false;
	// - (N dot P0 - d)
	// Doing origin - (0,0,0) because we need to convert from a
	// point to a vector
	Vector3 origin = r._origin - Point(0, 0, 0);
	double upper_term = -1 * (_normal.dotproduct(origin) - _d);
	double t = upper_term / dotproduct;

	// It is a valid intersection only when t is positive
	if (t <= 0) {
		return false;
	} else {
		Point pt = r._origin + (t * r._dir);
		in.set(t, pt, _normal);
		return true;
	}
}

bool Triangle::intersect(const Ray& ray, Intersection &in) {

	if (!this->checkbox(ray, in))
		return false;
	else {
		if (mode == BBOX_ONLY_MODE)
			return true;

		// Using the notation on the textbook. E.g: Vector abc has componets <a, b, c>
		Vector3 abc = _p1 - _p2; // a - b
		Vector3 def = _p1 - _p3; // a - c
		Vector3 ghi = ray._dir; // d
		double M, t, beta, gamma;
		// Symbolizing the entries in matrix A
		double a = abc._a, b = abc._b, c = abc._c;
		double d = def._a, e = def._b, f = def._c;
		double g = ghi._a, h = ghi._b, i = ghi._c;

		double ei_hf = e * i - h * f; // ei - hf
		double gf_di = g * f - d * i; // gf - di
		double dh_eg = d * h - e * g; // dh - eg

		M = a * ei_hf + b * gf_di + c * dh_eg; // |A|

		// Adopted from solution code
		if (M == 0)
			return false;

		Vector3 jkl = _p1 - ray._origin; // a - e

		double j = jkl._a, k = jkl._b, l = jkl._c;

		double ak_jb = a * k - j * b; // ak - jb
		double jc_al = j * c - a * l; // jc - al
		double bl_kc = b * l - k * c; // bl - kc

		t = -1.0 * (f * ak_jb + e * jc_al + d * bl_kc) / M; // Compute the value of t
		if (t <= 0.)
			return false;
		gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M; // Compute the value of gamma
		if (gamma < 0 or gamma > 1)
			return false;
		beta = (j * ei_hf + k * gf_di + l * dh_eg) / M; // Compute the value of beta
		if (beta < 0 or beta > (1 - gamma))
			return false;

		Point pt = ray._origin + (t * ray._dir);
		in.set(t, pt, _normal);
		return true;
	}
}

bool Surface::checkbox(const Ray& r, Intersection& in) const {

	if(mode == SLOW_MODE)
		return true;

	Point e = r._origin, min = _bbox.getMin(), max = _bbox.getMax();
	Vector3 d = r._dir;
	double tmax[3], tmin[3];
	double best_tmax = std::numeric_limits<double>::max();
	double best_tmin = 0.;

	// Looping over 3 dimensions
	for (int i = 0; i < 3; ++i) {
		double a = 1 / d[i];
		if (a >= 0) {
			tmin[i] = a * (min[i] - e[i]);
			tmax[i] = a * (max[i] - e[i]);
		} else {
			tmax[i] = a * (min[i] - e[i]);
			tmin[i] = a * (max[i] - e[i]);
		}
		if (tmin[i] > best_tmin)
			best_tmin = tmin[i];
		if (tmax[i] < best_tmax)
			best_tmax = tmax[i];
		if (best_tmin > best_tmax)
			return false;
	}

	Vector3 normal;
	// See which surface we intersected and set the normal in accordance
	if (best_tmin == tmin[0]) // x plane
		normal = (d[0] > 0) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
	else if (best_tmin == tmin[1]) // y plane
		normal = (d[1] > 0) ? Vector3(0, -1, 0) : Vector3(0, 1, 0);
	else // z plane
		normal = (d[2] > 0) ? Vector3(0, 0, -1) : Vector3(0, 0, 1);

	Point pt = e + best_tmin * d;
	in.set(best_tmin, pt, normal);
	return true;
}

bool BBoxNode::intersect(const Ray &r, Intersection &in) {
	if(checkbox(r, in)) {
		bool left_in, right_in;
		Intersection left_rec, right_rec;
		left_in = (_left != nullptr) and (_left->intersect(r, left_rec));
		right_in = (_right != nullptr) and (_right->intersect(r, right_rec));
		if(left_in and right_in) {
			if(left_rec.getT() < right_rec.getT()) {
				in = left_rec;
				_materialid = _left->materialid();
			} else {
				in = right_rec;
				_materialid = _right->materialid();
			}
			return true;
		} else if(left_in) {
			in = left_rec;
			_materialid = _left->materialid();
			return true;
		} else if(right_in) {
			in = right_rec;
			_materialid = _right->materialid();
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool comp_X(Surface *a, Surface *b) {
	Point ca = a->getBBox().getCenter();
	Point cb = b->getBBox().getCenter();
	return ca[0] < cb[0];
}
bool comp_Y(Surface *a, Surface *b) {
	Point ca = a->getBBox().getCenter();
	Point cb = b->getBBox().getCenter();
	return ca[1] < cb[1];
}
bool comp_Z(Surface *a, Surface *b) {
	Point ca = a->getBBox().getCenter();
	Point cb = b->getBBox().getCenter();
	return ca[2] < cb[2];
}

void BBoxNode::createTree(vector<Surface *>::iterator begin, vector<Surface *>::iterator end,  int AXIS) {
	size_t N = end - begin;
	if(N == 1) {
		// Only one surface in the list
		_left = *begin;
		_right = nullptr;
		_bbox = _left->getBBox();
	} else if(N == 2) {
		// Two surfaces in the list
		_left = *begin;
		_right = *(begin + 1);
		_bbox = combineBBoxes(_left->getBBox(), _right->getBBox());
	} else {
		// more than two surfaces in the list
		// sort the list by the centers of BBoxes
		switch(AXIS) {
		case 0:
			std::sort(begin, end, comp_X);
			break;
		case 1:
			std::sort(begin, end, comp_Y);
			break;
		case 2:
			std::sort(begin, end, comp_Z);
			break;
		default:
			break;
		}
		// Split the vector and recursive calls on both children
		BBoxNode *leftnode = new BBoxNode();
		BBoxNode *rightnode = new BBoxNode();
		leftnode->createTree(begin, begin + (N / 2), ((AXIS + 1) % 3));
		// TODO: do we have to do the plus one here?
		rightnode->createTree(begin + (N / 2), end, ((AXIS + 1) % 3));
		_left = leftnode;
		_right = rightnode;
		// combine the BBoxes of the children
		_bbox = combineBBoxes(_left->getBBox(), _right->getBBox());
	}
}

BBox BBoxNode::combineBBoxes(const BBox &b1, const BBox &b2) const {
//	double MIN = std::numeric_limits<double>::min();
//	double MAX = std::numeric_limits<double>::max();
//	Point min, max;
//	Point best_min(MAX, MAX, MAX);
//	Point best_max(MIN, MIN, MIN);
//	for(BBox *b : boxes) {
//		min = b->getMin();
//		max = b->getMax();
//		for(int i = 0; i < 3; i++) {
//			if(min[i] < best_min[i])
//				best_min[i] = min[i];
//			if(max[i] > best_max[i])
//				best_max[i] = max[i];
//		}
//	}
//	Point center = static_cast<Point> (0.5 * (best_max - best_min));
	Point b1_min = b1.getMin(), b1_max = b1.getMax(),
		  b2_min = b2.getMin(), b2_max = b2.getMax();
	Point best_min (std::min(b1_min[0], b2_min[0]),
					std::min(b1_min[1], b2_min[1]),
					std::min(b1_min[2], b2_min[2]));
	Point best_max (std::max(b1_max[0], b2_max[0]),
					std::max(b1_max[1], b2_max[1]),
					std::max(b1_max[2], b2_max[2]));
	Point center = static_cast<Point> (0.5 * (best_max - best_min));
	return BBox(best_min, best_max, center);
}

BBoxNode::~BBoxNode() {
	if(_left)
		delete _left;
	if(_right)
		delete _right;
}
