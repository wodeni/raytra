#ifndef RAYTRA_H
#define RAYTRA_H
#include "point.h"
#include "vector3.h"

class Ray {
    public:
        Point _origin;
        Vector3 _dir;
        double t; // The time of intersection, 0 if none
};

class Camera {
    public:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        int _pw, _ph;
        double _iw, _ih;

        Camera(Point&, Vector3&, float, float, float, int, int);
};

#endif /* RAYTRA_H */
