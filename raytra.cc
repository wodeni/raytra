#include "raytra.h"

Ray::Ray (Point origin, Vector3 dir) 
    : _origin(origin), _dir(dir), t(0)
{}

Camera::Camera (Point &pos, Vector3 &dir,
               float d, float iw, float ih,
               int pw, int ph)
    : _eye(pos), _d(d), _iw(iw), _ih(ih), _pw(pw), _ph(ph) 
{
    Vector3 up(0, 1, 0);
    _u =  dir.crossproduct(up);
    _v = _u.crossproduct(dir);
    _w = dir * -1.0;
}


std::ostream &operator<<(std::ostream &os, const Camera &c) {
    os << "Camera frame:" << c._u  << " " << c._v  << " " << c._w << std::endl;      
    return os;
}

/* Construct a ray give the (i,j)th pixel */
Ray Camera::construct_ray (int i, int j) {
    // Compute the coordinates for the puxel center
    double u = _iw * (i + 0.5) / _pw;
    double v = _ih * (j + 0.5) / _ph;

    Vector3 dir(-1 * _d, u, v);
    return Ray(_eye, dir);
}

Imf::Rgba *Camera::render(std::vector<Surface *> surfaces,  std::vector<Material *> materials) {
    for(auto i = 0; i < _ph; ++i) {
        for(auto j = 0; j < _pw; ++j) {
            Ray r = construct_ray(i, j); 

        }
    }
}
