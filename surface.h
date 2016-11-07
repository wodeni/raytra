/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef SURFACE_H 
#define SURFACE_H 
#include <limits>
#include "basemath.h"
#include "ray.h"

using namespace std;

class Intersection {
    public:
		Intersection() {
			_intersected = false;
			_t = std::numeric_limits<double>::max();
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
	BBox(const Point min, const Point max, const Point center)
	{ _min = min; _max = max; _center = center; }
	void addEpsilon() {
		double e = 1.0e-4;
		_min = Point (_min._a - e, _min._b - e, _min._c - e);
		_max = Point (_max._a + e, _max._b + e, _max._c + e);
	}
	Point getMin() const { return _min; }
	Point getMax() const { return _max; }
private:
	Point _min, _max, _center;
//	int _surfaceid;
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
    virtual bool intersect(const Ray&, Intersection&) const = 0;
    Surface () { _materialid = 0; }
    virtual ~Surface() {}
    virtual std::ostream& doprint(std::ostream &os) const = 0;
    int materialid() const { return _materialid; }
    void setmaterialid(const int materialid) { _materialid = materialid; }
    virtual bool checkbox(const Ray&, Intersection&);
protected:
    BBox _bbox;
    int _materialid; // the index of the material of this object
};

class Sphere : public Surface {
    public:
        Sphere(const Point &origin, const float &radius)
            : _origin(origin), _radius(radius)
        {
        	Point min(origin._a - radius, origin._b - radius, origin._c - radius);
        	Point max(origin._a + radius, origin._b + radius, origin._c + radius);
        	_bbox = BBox(min, max, origin);
    		_bbox.addEpsilon();
        }
        
        ~Sphere() {}
        virtual bool intersect(const Ray&, Intersection&) const override;
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
            : _normal(normal), _d(d) {}
        virtual bool intersect(const Ray&, Intersection&) const override;
        virtual std::ostream& doprint(std::ostream &os) const override {
            os << _normal <<  " " << _d;
            return os;
        }
        virtual bool checkbox(const Ray& r, Intersection& in) override { return true; }
    private:
        Vector3 _normal; 
        double _d;
};

class Triangle : public Surface {
public:
	Triangle (Point p1, Point p2, Point p3)
		: _p1(p1), _p2(p2), _p3(p3)
	{
		_normal = (p2 - p1).crossproduct(p3 - p1);
		_normal.normalize();
		double xmin = min( {p1._a, p2._a, p3._a} );
		double ymin = min( {p1._b, p2._b, p3._b} );
		double zmin = min( {p1._c, p2._c, p3._c} );
		double xmax = max( {p1._a, p2._a, p3._a} );
		double ymax = max( {p1._b, p2._b, p3._b} );
		double zmax = max( {p1._c, p2._c, p3._c} );
		Point min (xmin, ymin, zmin);
		Point max (xmax, ymax, zmax);
		// TODO: is the center correct?
		Point center ( (xmax - xmin) / 2,
					   (ymax - ymin) / 2,
					   (zmax - zmin) / 2);
		_bbox = BBox(min, max, center);
		_bbox.addEpsilon();
	}
    virtual bool intersect(const Ray&, Intersection&) const override;
    virtual std::ostream& doprint(std::ostream &os) const override {
        os << _p1 <<  " " << _p2 << " " << _p3;
        return os;
    }
private:
	Point _p1, _p2, _p3;
	Vector3 _normal;
};
#endif /* SURFACE_H */
