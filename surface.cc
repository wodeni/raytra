/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#include "surface.h"
#include <iostream>
#include <cmath>

Triangle::Triangle(Point p1, Point p2, Point p3)
		: _p1(p1), _p2(p2), _p3(p3)
{
	Vector3 abc = _p1 - _p2; // a - b
	Vector3 def = _p1 - _p3; // a - c
	a = abc._xyz[0], b = abc._xyz[1], c = abc._xyz[2];
	d = def._xyz[0], e = def._xyz[1], f = def._xyz[2];
	_normal = (-1.0 * abc).crossproduct(-1.0 * def);
	_normal.normalize();
	double xmin = min( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymin = min( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmin = min( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
	double xmax = max( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymax = max( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmax = max( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
//	Point min (xmin, ymin, zmin);
//	Point max (xmax, ymax, zmax);
//	// TODO: is the center correct?
//	Point center (
	double x = (xmax - xmin) * 0.5;
	double y = (ymax - ymin) * 0.5;
	double z = (zmax - zmin) * 0.5;

	_bbox = BBox(xmin, ymin, zmin, xmax, ymax, zmax, x, y, z);
	_bbox.addEpsilon();

}

bool Sphere::intersect(const Ray &r, Intersection &in, double &best_t) {
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
		if(t < best_t && t > STEP_NUM) best_t = t;
		return true;
	}
}

bool Plane::intersect(const Ray &r, Intersection &in, double &best_t) {

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
		if(t < best_t && t > STEP_NUM) best_t = t;
		return true;
	}
}

bool Triangle::intersect(const Ray& r, Intersection &in, double &best_t) {

	if (!this->checkbox(r, in))
		return false;
	else {
		if (mode == BBOX_ONLY_MODE)
			return true;

		double g = r._dir._xyz[0], h = r._dir._xyz[1], i = r._dir._xyz[2];

		double ei_hf = e * i - h * f; // ei - hf
		double gf_di = g * f - d * i; // gf - di
		double dh_eg = d * h - e * g; // dh - eg

		double M = a * ei_hf + b * gf_di + c * dh_eg; // |A|

		// Adopted from solution code
		if (M == 0)
			return false;

		Vector3 jkl = _p1 - r._origin; // a - e

		double j = jkl._xyz[0], k = jkl._xyz[1], l = jkl._xyz[2];

		double ak_jb = a * k - j * b; // ak - jb
		double jc_al = j * c - a * l; // jc - al
		double bl_kc = b * l - k * c; // bl - kc

		double t = -1.0 * (f * ak_jb + e * jc_al + d * bl_kc) / M; // Compute the value of t
		if (t <= 0.)
			return false;
		double gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M; // Compute the value of gamma
		if (gamma < 0 or gamma > 1)
			return false;
		double beta = (j * ei_hf + k * gf_di + l * dh_eg) / M; // Compute the value of beta
		if (beta < 0 or beta > (1 - gamma))
			return false;

		Point pt = r._origin + (t * r._dir);
		in.set(t, pt, _normal);
		if(t < best_t && t > STEP_NUM) best_t = t;
		return true;
	}
}

bool Surface::checkbox(const Ray& r, Intersection& in) const {

	if(mode == SLOW_MODE)
		return true;

	const Point &e = r._origin;
//	double tmax[3], tmin[3];
	double txmin, txmax;
//	double best_tmax = DOUBLE_MAX;
	double best_tmin = DOUBLE_MAX;

	// Looping over 3 dimensions
//	for (int i = 0; i < 3; ++i) {
//		double a = 1 / r._dir._xyz[i];
//		if (a >= 0) {
//			tmin[i] = a * (min._xyz[i] - e._xyz[i]);
//			tmax[i] = a * (max._xyz[i] - e._xyz[i]);
//		} else {
//			tmax[i] = a * (min._xyz[i] - e._xyz[i]);
//			tmin[i] = a * (max._xyz[i] - e._xyz[i]);
//		}
//		if (tmin[i] > best_tmin)
//			best_tmin = tmin[i];
//		if (tmax[i] < best_tmax)
//			best_tmax = tmax[i];
//		if (best_tmin > best_tmax)
//			return false;
//	}
	double a = 1 / r._dir._xyz[0];
	if(a >= 0) {
		txmin = a * (_bbox._xmin - e._xyz[0]);
		txmax = a * (_bbox._xmax - e._xyz[0]);
	} else {
		txmax = a * (_bbox._xmin - e._xyz[0]);
		txmin = a * (_bbox._xmax - e._xyz[0]);
	}
	if(txmin < best_tmin)
		best_tmin = txmin;
	double tymin, tymax;
	double b = 1 / r._dir._xyz[1];
	if(b >= 0) {
		tymin = b * (_bbox._ymin - e._xyz[1]);
		tymax = b * (_bbox._ymax - e._xyz[1]);
	} else {
		tymax = b * (_bbox._ymin - e._xyz[1]);
		tymin = b * (_bbox._ymax - e._xyz[1]);
	}
	if(txmin > tymax || tymin > txmax)
		return false;
	if(tymin < best_tmin)
			best_tmin = tymin;
	double tzmin, tzmax;
	double c = 1 / r._dir._xyz[2];
	if(c >= 0) {
		tzmin = c * (_bbox._zmin - e._xyz[2]);
		tzmax = c * (_bbox._zmax - e._xyz[2]);
	} else {
		tzmax = c * (_bbox._zmin - e._xyz[2]);
		tzmin = c * (_bbox._zmax - e._xyz[2]);
	}

	if(txmin > tzmax || tzmin > txmax)
		return false;
	if(tymin > tzmax || tzmin > tymax)
		return false;
	if(tzmin < best_tmin)
			best_tmin = tzmin;
//	double best_tmin = std::min(txmin, std::min(tymin, tzmin));

	if(mode == BBOX_ONLY_MODE) {
		Vector3 normal;
//		Vector3 d = r._dir;
		// See which surface we intersected and set the normal in accordance
		if (best_tmin == txmin) // x plane
			normal = (r._dir._xyz[0] > 0) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
		else if (best_tmin == tymin) // y plane
			normal = (r._dir._xyz[1] > 0) ? Vector3(0, -1, 0) : Vector3(0, 1, 0);
		else // z plane
			normal = (r._dir._xyz[2] > 0) ? Vector3(0, 0, -1) : Vector3(0, 0, 1);

		Point pt = e + best_tmin * r._dir;
		in.set(best_tmin, pt, normal);
	}
	in.setT(best_tmin);
	return true;
}

bool BBoxNode::intersect(const Ray &r, Intersection &in, double &best_t) {
	if(checkbox(r, in) and in.getT() < best_t) {
		bool left_in, right_in;
		Intersection left_rec, right_rec;
		left_in = (_left != nullptr) and (_left->intersect(r, left_rec, best_t)) and (left_rec.getT() > STEP_NUM);
		right_in = (_right != nullptr) and (_right->intersect(r, right_rec, best_t)) and (right_rec.getT() > STEP_NUM);
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
	return a->_bbox._x < b->_bbox._x;
}
bool comp_Y(Surface *a, Surface *b) {
	return a->_bbox._y < b->_bbox._y;
}
bool comp_Z(Surface *a, Surface *b) {
	return a->_bbox._z < b->_bbox._z;
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

//	Point b1_min = b1._min, b1_max = b1._max,
//		  b2_min = b2._min, b2_max = b2._max;
//	Point best_min (std::min(b1_min[0], b2_min[0]),
//					std::min(b1_min[1], b2_min[1]),
//					std::min(b1_min[2], b2_min[2]));
//	Point best_max (std::max(b1_max[0], b2_max[0]),
//					std::max(b1_max[1], b2_max[1]),
//					std::max(b1_max[2], b2_max[2]));
//	Point center = static_cast<Point> (0.5 * (best_max - best_min));
//	return BBox(best_min, best_max, center);
	double xi = std::min(b1._xmin, b2._xmin);
	double yi = std::min(b1._ymin, b2._ymin);
	double zi = std::min(b1._zmin, b2._zmin);
	double xa = std::max(b1._xmax, b2._xmax);
	double ya = std::max(b1._ymax, b2._ymax);
	double za = std::max(b1._zmax, b2._zmax);
	double x = 0.5 * (xa - xi);
	double y = 0.5 * (ya - yi);
	double z = 0.5 * (za - zi);

	return BBox(xi, yi, zi, xa, ya, za, x, y, z);
}

BBoxNode::~BBoxNode() {
	if(_left)
		delete _left;
	if(_right)
		delete _right;
}
