/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef SURFACE_H 
#define SURFACE_H 
#include "basemath.h"
#include "ray.h"

class P_Light {
    public:
        P_Light(Point position, Vector3 color) 
            : _position(position), _color(color) {}
        Vector3 getColor() const { return _color; }
        Point getPosition() const { return _position; }
    private:
        Point _position;
        Vector3 _color;
};

class Intersection {
    public:
        int surfaceid() const { return _surfaceid; }
        void setsurfaceid(const int id) { _surfaceid = id; }
        bool Intersected() const { return _intersected; }
        void setIntersected(const bool& flag) { _intersected = flag; }
        double getT() const { return std::min(_t1, _t2); }
        void setTs(const double &t1, const double &t2) { _t1 = t1; _t2 = t2; }
        Point getIntersectionPoint() const { return _intersectionPoint; }
        void setIntersectionPoint(const Point p) { _intersectionPoint = p; }
        Vector3 getNormal() const { return _normal; } 
        void setNormal(const Vector3 v) { _normal = v; }
        /* Used by std::sort() to sort the vector of intersections */
        bool operator<(const Intersection &rhs) const {
            return std::min(_t1, _t2) < std::min(rhs._t1, rhs._t2);
        }
    private:
        double _t1, _t2; // the t values of the intersection point(s)
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
    virtual ~Surface() {}
    virtual std::ostream& doprint(std::ostream &os) const = 0;
    int materialid() const { return _materialid; }
    void setmaterialid(const int materialid) { _materialid = materialid; }
private:
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
#endif /* SURFACE_H */
