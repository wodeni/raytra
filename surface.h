/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef SURFACE_H 
#define SURFACE_H 
#include "basemath.h"
#include "ray.h"

class Intersection {
    public:
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
        bool operator<(const Intersection &rhs) const {
            return _t< rhs._t;
        }
    private:
        double _t; // the t value of the intersection point(s)
        Vector3 _normal; // the geometric normal
        Point _intersectionPoint; // the intersection points
        int _surfaceid; // used to look up material
        bool _intersected; // boolean showing whether there is an intersection
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
    virtual Intersection intersect(Ray&) = 0;
    Surface () { _materialid = 0; }
    virtual ~Surface() {}
    virtual std::ostream& doprint(std::ostream &os) const = 0;
    int materialid() const { return _materialid; }
    void setmaterialid(const int materialid) { _materialid = materialid; }
protected:
    int _materialid; // the index of the material of this object
};



class Sphere : public Surface {
    public:
        Sphere(const Point &origin, const float &radius)
            : _origin(origin), _radius(radius)
        {}
        
        ~Sphere() {}
        virtual Intersection intersect(Ray&);
        virtual std::ostream& doprint(std::ostream &os) const {
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
        virtual Intersection intersect(Ray&);
        virtual std::ostream& doprint(std::ostream &os) const {
            os << _normal <<  " " << _d;
            return os;
        }
    private:
        Vector3 _normal; 
        double _d;
};

class Triangle : public Surface {
public:
	Triangle (Point p1, Point p2, Point p3)
		: _p1(p1), _p2(p2), _p3(p3) {}
	virtual Intersection intersect(Ray&);
    virtual std::ostream& doprint(std::ostream &os) const {
        os << _p1 <<  " " << _p2 << " " << _p3;
        return os;
    }
private:
	Point _p1, _p2, _p3;
//	double _t0, _t1; // Bounding values for intersection computation
//				   // Placed here because we could not change the
//				   // function signature of intersection()
};
#endif /* SURFACE_H */
