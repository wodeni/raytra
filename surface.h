#ifndef SURFACE_H 
#define SURFACE_H 
#include "basemath.h"
#include "ray.h"

/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
class Intersection {
    public:
        int surfaceid() const { return _surfaceid; }
        void setsurfaceid(const int id) { _surfaceid = id; }
        bool Intersected() const { return _intersected; }
        void setIntersected(const bool& flag) { _intersected = flag; }
        void setTs(const double &t1, const double &t2) {
            _t1 = t1; _t2 = t2;
        }
        /* Used by std::sort() to sort the vector of intersections */
        bool operator<(const Intersection &rhs) const {
            return std::min(_t1, _t2) < std::min(rhs._t1, rhs._t2);
        }
    private:
        double _t1, _t2; // the t values of the intersection point(s)
        Vector3 _normal; // the geometric normal
        Point _p1, _p2; // the intersection points
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
        
        ~Sphere() { std::cout << "Sphere deallocated." << std::endl; }
        virtual Intersection intersect(Ray&);
        virtual std::ostream& doprint(std::ostream &os) const {
            os << _origin <<  " " << _radius;
            return os;
        }
    private:
        Point _origin;
        float _radius;
};
#endif /* SURFACE_H */
