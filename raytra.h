/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#ifndef RAYTRA_H
#define RAYTRA_H
#include <vector>
#include <algorithm>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include "material.h"
#include "basemath.h"
#include "ray.h"

class Camera {

    friend std::ostream &operator<<(std::ostream &, const Camera &);

    public:
        Camera() {}
        void initCamera (Point&, Vector3&, float&, 
                float&, float&, int&, int&, 
                std::vector<Material *>&, std::vector<Surface *>&);
        ~Camera();
        Ray construct_ray (int, int);
        void render(const char filename[]);
        void writeRgba(const char filename[], Imf::Rgba *pixels);
        void setmaterials(const std::vector<Material *> &m) {
            _materials = m;
        }
        void setsurfaces(const std::vector<Surface *> &s) {
            _surfaces = s;
        }
    private:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        double _iw, _ih;
        int _pw, _ph;

        std::vector<Material *> _materials;
        std::vector<Surface *> _surfaces;
};

#endif /* RAYTRA_H */
