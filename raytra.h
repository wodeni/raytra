/**
 * @author: Wode Ni
 * @version: 2016/10/12
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
                float&, float&, int&, int&);
        ~Camera();
        Ray construct_ray (int, int);
        void render(const char filename[],
                std::vector< Surface *> &surfaces,
                std::vector< Material *> &materials,
                 std::vector< P_Light * > &lights);
        void writeRgba(const char filename[], Imf::Rgba *pixels);
    private:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        double _iw, _ih;
        int _pw, _ph;
};

#endif /* RAYTRA_H */
