#ifndef RAYTRA_H
#define RAYTRA_H
#include <vector>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include "material.h"
#include "point.h"
#include "vector3.h"

class Ray {
    public:
        Point _origin;
        Vector3 _dir;
        Ray (Point, Vector3);
};

class Surface;

class Camera {

    // for testing purpose
    friend std::ostream &operator<<(std::ostream &, const Camera &);

    public:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        double _iw, _ih;
        int _pw, _ph;

        Camera (Point&, Vector3&, float, float, float, int, int);
        Ray construct_ray (int, int);
void render(Imf::Array2D<Imf::Rgba> &, std::vector<Surface *>&, std::vector<Material *>&);
        void writeRgba(const char filename[], Imf::Rgba *pixels);
};

#endif /* RAYTRA_H */
