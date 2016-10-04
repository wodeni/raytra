#ifndef SPHERE_H
#define SPHERE_H
#include "point.h"
#include "surface.h"

class Sphere : public Surface {
    public:
        Point origin;
        float radius;
        Sphere(Point origin, float radius);
};


#endif /* SPHERE_H */
