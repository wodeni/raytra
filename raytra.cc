#include "raytra.h"
#include "surface.h"
#include <iostream>
#define DEBUG 1 
using namespace Imf;
using namespace std;

Ray::Ray (Point origin, Vector3 dir) 
    : _origin(origin), _dir(dir)
{}

Camera::Camera (Point &pos, Vector3 &dir,
        float d, float iw, float ih,
        int pw, int ph)
    : _eye(pos), _d(d), _iw(iw), _ih(ih), _pw(pw), _ph(ph) 
{
    Vector3 up(0, 1, 0);
    _u = dir.crossproduct(up).normalize();
    _v = _u.crossproduct(dir).normalize();
    _w = (dir * -1.0).normalize();
}


std::ostream &operator<<(std::ostream &os, const Camera &c) {
    os << "Camera frame:" << c._u  << " " << c._v  << " " << c._w << std::endl;      
    return os;
}

/* Construct a ray given the (i,j)th pixel */
Ray Camera::construct_ray (int i, int j) {
    // Compute the coordinates of the pixel center
    double r, l, t, b, u, v;
    Vector3 dir;
    r = _iw / 2.0;
    l = -1.0 * r;
    t = _ih / 2.0;
    b = -1.0 * t;
    u = l + _iw * (i + 0.5) / _pw;
    v = b + _ih * (j + 0.5) / _ph;

    dir = (u * _u + v * _v + (-1 * _d) * (_w)).normalize();
    return Ray(_eye, dir);
}

void Camera::render(Imf::Array2D<Imf::Rgba> &res, std::vector<Surface *> &surfaces,  std::vector<Material *> &materials) {
#ifdef DEBUG
    cout << materials[0]->_diff << endl;
#endif
    res.resizeErase(_ph, _pw);
    for(int i = 0; i < _pw; ++i) { 
        for(int j = 0; j < _ph; ++j) {
            Ray r = construct_ray(i,j); 
            std::vector<Intersection> in_arr(surfaces.size());
            Rgba& px = res[_ph - 1 - j][i]; // the current px in the picture

            // Intersect the scene
            for(int k = 0; k < surfaces.size(); ++k) {
                Intersection tmp;
                tmp = surfaces[k]->intersect(r);
                tmp.surfaceid = k;
                in_arr[k] = tmp;
            }  

            // From the result of intersecting, color the pixel
            for(int p = 0; p < in_arr.size(); ++p) {
                if(in_arr[p].intersected) {
                    // Look up material
                    Material m = *materials[in_arr[p].surfaceid];
                    px.r = m._diff._a; 
                    px.g = m._diff._b; 
                    px.b = m._diff._c; 
                }
            }
        }
    }
}

void Camera::writeRgba(const char filename[], Rgba *pixels) {
    RgbaOutputFile file (filename, _pw, _ph, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, _pw);
    file.writePixels (_ph);
}

