#ifndef SURFACE_H 
#define SURFACE_H 
#include "point.h"
#include "vector3.h"
#include "raytra.h"

class Intersection {
    public:
        int intersected; // boolean showing whether there is an intersection
        double _t1, _t2; // the t values of the intersection point(s)
        Vector3 _normal; // the geometric normal
        Point _p1, _p2; // the intersection points
        int surfaceid; // used to look up material
};

/**
 * Superclass for objects in the scene
 */
class Surface {
    public:
        virtual Intersection intersect(Ray) = 0;
};



class Sphere : public Surface {
    friend std::ostream &operator<<(std::ostream&, Sphere&);
    public:
        Point _origin;
        float _radius;
        Sphere(Point origin, float radius);
        Intersection intersect(Ray);
};
#endif /* SURFACE_H */
