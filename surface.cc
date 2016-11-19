/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#include "surface.h"
#include <iostream>
#include <cmath>

int COUNT = 0;
int COUNT2 = 0;

Triangle::Triangle(Point p1, Point p2, Point p3)
		: _p1(p1), _p2(p2), _p3(p3)
{
	Vector3 abc = _p1 - _p2; // a - b
	Vector3 def = _p1 - _p3; // a - c
	a = abc._xyz[0], b = abc._xyz[1], c = abc._xyz[2];
	d = def._xyz[0], e = def._xyz[1], f = def._xyz[2];
	_geometricnormal = (-1.0 * abc).crossproduct(-1.0 * def);
	_geometricnormal.normalize();
	double xmin = min( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymin = min( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmin = min( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
	double xmax = max( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymax = max( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmax = max( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
	double x = (xmax + xmin) * 0.5;
	double y = (ymax + ymin) * 0.5;
	double z = (zmax + zmin) * 0.5;
	_isMesh = false;
	_normals = nullptr;
	_v1 = _v2 = _v3 = -1;

	_bbox = BBox(xmin, ymin, zmin, xmax, ymax, zmax, x, y, z);
	_bbox.addEpsilon();

}

Triangle::Triangle (Point p1, Point p2, Point p3, int v1, int v2, int v3, vector<Vector3 *> *normals, bool isMesh)
		: _p1(p1), _p2(p2), _p3(p3)
{
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;
//	_verts = verts;
	_normals = normals;
	_isMesh = isMesh;
//	Point p1(_verts[v1], _verts[v1 + 1], _verts[v1 + 2]);
//	Point p2(_verts[v2], _verts[v2 + 1], _verts[v2 + 2]);
//	Point p3(_verts[v3], _verts[v3 + 1], _verts[v3 + 2]);

	Vector3 abc = p1 - p2; // a - b
	Vector3 def = p1 - p3; // a - c
	a = abc._xyz[0], b = abc._xyz[1], c = abc._xyz[2];
	d = def._xyz[0], e = def._xyz[1], f = def._xyz[2];
	_geometricnormal = (-1.0 * abc).crossproduct(-1.0 * def);
	_geometricnormal.normalize();
	double xmin = min( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymin = min( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmin = min( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
	double xmax = max( {p1._xyz[0], p2._xyz[0], p3._xyz[0]} );
	double ymax = max( {p1._xyz[1], p2._xyz[1], p3._xyz[1]} );
	double zmax = max( {p1._xyz[2], p2._xyz[2], p3._xyz[2]} );
	double x = (xmax + xmin) * 0.5;
	double y = (ymax + ymin) * 0.5;
	double z = (zmax + zmin) * 0.5;

	_bbox = BBox(xmin, ymin, zmin, xmax, ymax, zmax, x, y, z);
	_bbox.addEpsilon();
}


bool Sphere::intersect(const Ray &r, Intersection &in, double &best_t) {
	if (!_bbox.checkbox(r, in))
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

	if (!_bbox.checkbox(r, in))
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

//		Point _p1(_verts[_v1], _verts[_v1 + 1], _verts[_v1 + 2]);
//		Point p2(_verts[v2], _verts[v2 + 1], _verts[v2 + 2]);
//		Point p3(_verts[v3], _verts[v3 + 1], _verts[v3 + 2]);

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

		// Intersection FOUND
		Vector3 normal(0, 0, 0); // the normal to be returned
		Point pt = r._origin + (t * r._dir); // Intersection point to be returned

		// If this trig is a part of a mesh, do smooth normal
		if(_isMesh) {
#if USE_SMOOTH_NORMAL
			normal += *_normals->at(_v1) * (1.0 - beta - gamma);
			normal += *_normals->at(_v2) * beta;
			normal += *_normals->at(_v3) * gamma;
			normal.normalize();
#else
			normal = _geometricnormal;
#endif
		} else {
			normal = _geometricnormal;
		}
		in.setMaterialId(_materialid);

#if BACKSIDE_YELLOW
		Vector3 e = -1.0 * r._dir; // Direction of the ray already normalized
		// If the camera is pointing to the back of the surface, color it yellow
		if (e.dotproduct(_geometricnormal) < 0.) { // using the GROMETRIC normal here
			in.setMaterialId(YELLOW_INDEX);
			normal = -1.0 * normal;
		}
#endif
		in.set(t, pt, normal);
		if(t < best_t && t > STEP_NUM) best_t = t;
		return true;
	}
}

bool BBox::checkbox(const Ray& r, Intersection& in) const {

	if(mode == SLOW_MODE)
		return true;

	const Point &e = r._origin;
	double txmin, txmax;
	double best_tmin = 0;

	double a = 1 / r._dir._xyz[0];
	if(a >= 0) {
		txmin = a * (_xmin - e._xyz[0]);
		txmax = a * (_xmax - e._xyz[0]);
	} else {
		txmax = a * (_xmin - e._xyz[0]);
		txmin = a * (_xmax - e._xyz[0]);
	}
	if(txmin > best_tmin)
		best_tmin = txmin;
	double tymin, tymax;
	double b = 1 / r._dir._xyz[1];
	if(b >= 0) {
		tymin = b * (_ymin - e._xyz[1]);
		tymax = b * (_ymax - e._xyz[1]);
	} else {
		tymax = b * (_ymin - e._xyz[1]);
		tymin = b * (_ymax - e._xyz[1]);
	}
	if(txmin > tymax || tymin > txmax)
		return false;
	if(tymin > best_tmin)
		best_tmin = tymin;
	double tzmin, tzmax;
	double c = 1 / r._dir._xyz[2];
	if(c >= 0) {
		tzmin = c * (_zmin - e._xyz[2]);
		tzmax = c * (_zmax - e._xyz[2]);
	} else {
		tzmax = c * (_zmin - e._xyz[2]);
		tzmin = c * (_zmax - e._xyz[2]);
	}

	if(txmin > tzmax || tzmin > txmax)
		return false;
	if(tymin > tzmax || tzmin > tymax)
		return false;
	if(tzmin > best_tmin)
		best_tmin = tzmin;

	if(mode == BBOX_ONLY_MODE) {
		Vector3 normal;
		// See which surface we intersected and set the normal in accordance
		if (best_tmin == txmin) // x plane
			normal = (r._dir._xyz[0] > 0) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
		else if (best_tmin == tymin) // y plane
			normal = (r._dir._xyz[1] > 0) ? Vector3(0, -1, 0) : Vector3(0, 1, 0);
		else // z plane
			normal = (r._dir._xyz[2] > 0) ? Vector3(0, 0, -1) : Vector3(0, 0, 1);
		Point pt = e + best_tmin * r._dir;
		in.set(best_tmin, pt, normal);
		return true;
	}
	in.setT(best_tmin);
	if(_materialid != -1)
		in.setMaterialId(_materialid);
	return true;
}

bool BBoxNode::checkshadow(const Ray& r, Intersection& in, double& best_t) {
	if(_bbox.checkbox(r, in) and in.getT() < best_t) {
		Intersection left_rec;
		bool left_in = (_left != nullptr) and (_left->checkshadow(r, left_rec, best_t)) and (left_rec.getT() > STEP_NUM);
		if(left_in) {
			in = left_rec;
			return true;
		}
		Intersection right_rec;
		bool right_in = (_right != nullptr) and (_right->checkshadow(r, right_rec, best_t)) and (right_rec.getT() > STEP_NUM);
		if(right_in) {
			in = right_rec;
			return true;
		}
	}
	return false;
}


bool BBoxNode::intersect(const Ray &r, Intersection &in, double &best_t) {
	if(_bbox.checkbox(r, in) and in.getT() < best_t) {
		bool left_in, right_in;
		Intersection left_rec, right_rec;
		left_in = (_left != nullptr) and (_left->intersect(r, left_rec, best_t)) and (left_rec.getT() > STEP_NUM);
		right_in = (_right != nullptr) and (_right->intersect(r, right_rec, best_t)) and (right_rec.getT() > STEP_NUM);
		if(left_in and right_in) {
			if(left_rec.getT() < right_rec.getT()) {
				in = left_rec;
//				_materialid = _left->materialid();
			} else {
				in = right_rec;
//				_materialid = _right->materialid();
			}
			return true;
		} else if(left_in) {
			in = left_rec;
//			_materialid = _left->materialid();
			return true;
		} else if(right_in) {
			in = right_rec;
//			_materialid = _right->materialid();
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

//void BBoxNode::createTree(vector<Surface *>::iterator begin, vector<Surface *>::iterator end,  int AXIS) {
void BBoxNode::createTree(vector<Surface *> &list, int l, int r, int AXIS) {
	if(l == r) {
		// Only one surface in the list
		_left = list[l];
		_right = nullptr;
		_bbox = _left->getBBox();
		_left->_bbox.setMaterialId(_left->materialid());
	} else if(l < r) {
		if(r == l + 1) {
			// Two surfaces in the list
			_left = list[l];
			_right = list[r];
			_bbox = combineBBoxes(_left->getBBox(), _right->getBBox());
			_left->_bbox.setMaterialId(_left->materialid());
			_right->_bbox.setMaterialId(_right->materialid());
		} else {
			int mid = l + (r - l) / 2;
			// more than two surfaces in the list
			// sort the list by the centers of BBoxes
			switch(AXIS) {
			case 0:
				std::sort(list.begin() + l, list.begin() + r + 1, comp_X);
				break;
			case 1:
				std::sort(list.begin() + l, list.begin() + r + 1, comp_Y);
				break;
			case 2:
				std::sort(list.begin() + l, list.begin() + r + 1, comp_Z);
				break;
			default:
				break;
			}
			// Split the vector and recursive calls on both children
			BBoxNode *leftnode = new BBoxNode();
			BBoxNode *rightnode = new BBoxNode();
			leftnode->createTree(list, l, mid, ((AXIS + 1) % 3));
			rightnode->createTree(list, mid + 1, r, ((AXIS + 1) % 3));
			_left = leftnode;
			_right = rightnode;
			// combine the BBoxes of the children
			_bbox = combineBBoxes(_left->getBBox(), _right->getBBox());
		}
	}
}

int BBoxNode::countNodes() {
	int res = 1;
	BBoxNode* tmp = nullptr;
	if(_left != nullptr) {
		if(!(tmp = dynamic_cast<BBoxNode *>(_left)))
			res += 1;
		else
			res += tmp->countNodes();
	}
	if(_right != nullptr) {
		if(!(tmp = dynamic_cast<BBoxNode *>(_right)))
			res += 1;
		else
			res += tmp->countNodes();
	}
	return res;
}

BBox BBoxNode::combineBBoxes(const BBox &b1, const BBox &b2) const {

	double xi = std::min(b1._xmin, b2._xmin);
	double yi = std::min(b1._ymin, b2._ymin);
	double zi = std::min(b1._zmin, b2._zmin);
	double xa = std::max(b1._xmax, b2._xmax);
	double ya = std::max(b1._ymax, b2._ymax);
	double za = std::max(b1._zmax, b2._zmax);
	double x = 0.5 * (xa + xi);
	double y = 0.5 * (ya + yi);
	double z = 0.5 * (za + zi);

	return BBox(xi, yi, zi, xa, ya, za, x, y, z);
}

BBoxNode::~BBoxNode() {
	if(_left)
		delete _left;
	if(_right)
		delete _right;
}
