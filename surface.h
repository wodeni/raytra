/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef SURFACE_H 
#define SURFACE_H 
#include <vector>
#include <algorithm>
#include "basemath.h"
#include "ray.h"
#include "constants.h"


using namespace std;

class Intersection {
    public:
		Intersection() {
			_intersected = false;
			_t = DOUBLE_MAX;
			_intersectionPoint = Point();
			_surfaceid = -1;
		}
		void set(const double t, const Point &pt, const Vector3 &normal) {
			_t = t;  _normal = normal; _intersected = true;
			_intersectionPoint = pt;
		}
		int surfaceid() const { return _surfaceid; }
        void setsurfaceid(const int id) { _surfaceid = id; }
        bool Intersected() const { return _intersected; }
        void setIntersected(const bool& flag) { _intersected = flag; }
        double getT() const { return _t; }
        void setT(const double &t) { _t = t; }
        Point getIntersectionPoint() const { return _intersectionPoint; }
        void setIntersectionPoint(const Point p) { _intersectionPoint = p; }
        Vector3 getNormal() const { return _normal; } 
        void setNormal(const Vector3 v) { _normal = v; }
        /* Used by std::sort() to sort the vector of intersections */
//        bool operator<(const Intersection &rhs) const {
//            return _t< rhs._t;
//        }
    private:
        double _t; // the t value of the intersection point(s)
        Vector3 _normal; // the geometric normal
        Point _intersectionPoint; // the intersection points
        int _surfaceid; // used to look up material
        bool _intersected; // boolean showing whether there is an intersection
};

/**
 * The bounding box class for all surfaces but plane
 */
class BBox {
public:
	BBox () {}
	BBox(double xi, double yi, double zi, double xa, double ya, double za, double x, double y, double z)
	{
		_xmin = xi; _ymin = yi; _zmin = zi;
		_xmax = xa; _ymax = ya; _zmax = za;
		_x = x; _y = y; _z = z;
	}
	void addEpsilon() {
		_xmin -= STEP_NUM; _ymin -= STEP_NUM; _zmin -= STEP_NUM;
		_xmax += STEP_NUM; _ymax += STEP_NUM; _zmax += STEP_NUM;
	}
    bool checkbox(const Ray&, Intersection&) const;
	double _xmin, _xmax, _ymin, _ymax, _zmin, _zmax;
	double _x, _y, _z;
    friend std::ostream &operator<<(std::ostream &os, const BBox &b) {
    	os << b._xmin <<  " " << b._xmax << " " << b._ymin << " " << b._ymax << " " << b._zmin << " " << b._zmax<< endl;
        return os;
    }
};


/**
* Superclass for objects in the scene
*/
class Surface {
    friend std::ostream &operator<<(std::ostream &os, const Surface &s) {
        s.doprint(os);
        return os;
    }
public:
    virtual bool intersect(const Ray&, Intersection&, double&) = 0;
    Surface () { _materialid = 0; }
    virtual ~Surface() {}
    virtual std::ostream& doprint(std::ostream &os) const = 0;
    int materialid() const { return _materialid; }
    void setmaterialid(const int materialid) { _materialid = materialid; }
    virtual bool checkshadow(const Ray& r, Intersection& in, double& best_t) {
    	return intersect(r, in, best_t);
    }

//    bool checkbox(const Ray&, Intersection&) const;
    BBox getBBox() { return _bbox; }
    BBox _bbox; // TODO: is this OOP?

protected:
    int _materialid; // the index of the material of this object
};

class Sphere : public Surface {
    public:
        Sphere(const Point &origin, const float &radius)
            : _origin(origin), _radius(radius)
        {
//        	Point min(origin._xyz[0] - radius, origin._xyz[1] - radius, origin._xyz[2] - radius);
//        	Point max();
        	_bbox = BBox(
        			origin._xyz[0] - radius, origin._xyz[1] - radius, origin._xyz[2] - radius,
        			origin._xyz[0] + radius, origin._xyz[1] + radius, origin._xyz[2] + radius,
					origin._xyz[0], origin._xyz[1], origin._xyz[2]);
    		_bbox.addEpsilon();
        }
        
        ~Sphere() {}
        virtual bool intersect(const Ray&, Intersection&, double&) override;
        virtual std::ostream& doprint(std::ostream &os) const override {
            os << _origin <<  " " << _radius;
            return os;
        }
    private:
        Point _origin;
        float _radius;
};

class Plane : public Surface {
    public:
        Plane(Vector3 normal, double d) 
            : _normal(normal), _d(d)
    	{
    	}
        virtual bool intersect(const Ray&, Intersection&, double&) override;
//        virtual bool checkbox(const Ray&, Intersection&) const override {
//        	if(mode == SLOW_MODE)
//        		return true;
//        	else
//        		return false;
//        }
        virtual std::ostream& doprint(std::ostream &os) const override {
            os << _normal <<  " " << _d;
            return os;
        }
    private:
        Vector3 _normal; 
        double _d;
};

class Triangle : public Surface {
public:
	Triangle (Point p1, Point p2, Point p3);
    virtual bool intersect(const Ray&, Intersection&, double&) override;
    virtual std::ostream& doprint(std::ostream &os) const override {
        os << _p1 <<  " " << _p2 << " " << _p3;
        return os;
    }
private:
	Point _p1, _p2, _p3;
	double a, b, c, d, e, f;
	Vector3 _normal;
};


/**
 * The bounding box node class for BVH tree acceleration
 */
class BBoxNode : public Surface {
public:
    virtual ~BBoxNode();
	virtual bool intersect(const Ray&, Intersection&, double&) override;
//	bool checkshadow(const Ray&, Intersection&, double&);
virtual bool checkshadow(const Ray&, Intersection&, double&) override;
    virtual std::ostream& doprint(std::ostream &os) const override {
    	cout << _bbox << endl;
    	return os;
    }
    void createTree(vector<Surface *>::iterator begin, vector<Surface *>::iterator end, int AXIS);
    BBox combineBBoxes(const BBox &, const BBox &) const;
    int countNodes();
//private:
	Surface *_left;
	Surface *_right;
};
#endif /* SURFACE_H */
