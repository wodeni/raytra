#ifndef RAYTRA_H
#define RAYTRA_H
#include <vector>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include "material.h"
#include "surface.h"
#include "point.h"
#include "vector3.h"

class Ray {
    public:
        Point _origin;
        Vector3 _dir;
        double t; // The time of intersection, 0 if none
        Ray (Point, Vector3);
};

class Camera {

    // for testing purpose
    friend std::ostream &operator<<(std::ostream &, const Camera &);

    public:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        int _pw, _ph;
        double _iw, _ih;

        Camera (Point&, Vector3&, float, float, float, int, int);
        Ray construct_ray (int, int);
        Imf::Rgba *render(std::vector<Surface *>,  std::vector<Material *>);
};

#endif /* RAYTRA_H */
