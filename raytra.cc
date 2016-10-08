#include "parse.h"
#include "raytra.h"
#include "surface.h"
#include <iostream>
#define DEBUG 0 
using namespace Imf;
using namespace std;

void Camera::initCamera (Point &pos, Vector3 &dir,
        float &d, float &iw, float &ih,
        int &pw, int &ph, 
        vector<Material *> &materials,
        vector<Surface *> &surfaces)
{
    _eye = pos;
    _d = d;
    _iw = iw;
    _ih = ih;
    _pw = pw;
    _ph = ph;
    Vector3 up(0, 1, 0);
    _u = dir.crossproduct(up).normalize();
    _v = _u.crossproduct(dir).normalize();
    _w = (dir * -1.0).normalize();
    _materials = materials;
    _surfaces = surfaces;
#if DEBUG 
    cout << *this ;
#endif
}

Camera::~Camera() {
#if DEBUG
    cout << "Destructing Camera..." << endl;
#endif
    for(std::size_t i = 0; i < _materials.size(); ++i) {
        delete _materials[i];
    }
    _materials.clear();
    for(std::size_t i = 0; i < _surfaces.size(); ++i) {
        delete _surfaces[i];
    }
    _surfaces.clear();
}

std::ostream &operator<<(std::ostream &os, const Camera &c) {
    os << "Camera frame:" << c._u  << " " << c._v  << " " << c._w << std::endl;      
    os << "# of objects in the scene: " << c._surfaces.size() << endl;
    os << "# of materials in the scene: " << c._materials.size() << endl;
    
    // TODO: set up printing mechanism for these two classes
    // os << "Printing out the materials: " << _materials << std::endl;
    // os << "Printing out the surfaces: " << _surfaces << std::endl;

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

void Camera::render(const char filename[]) {

    Array2D<Rgba> res;
    res.resizeErase(_ph, _pw);

    for(int i = 0; i < _pw; ++i) { 
        for(int j = 0; j < _ph; ++j) {
            // Construct ray
            Ray r = construct_ray(i,j); 
            std::vector<Intersection> in_arr;
            Rgba& px = res[_ph - 1 - j][i]; // the current px in the picture

            // Intersect the scene
            for(std::size_t k = 0; k < _surfaces.size(); ++k) {
                Intersection tmp;
                tmp = _surfaces[k]->intersect(r);
                if(tmp.Intersected()) {
                    tmp.setsurfaceid(k);
                    in_arr.push_back(tmp);
                }
            }  

            if(in_arr.size() != 0) {
                // From the result of intersecting, color the pixel
                std::sort(in_arr.begin(), in_arr.end());
                // Pick the closest object 
                Surface* closestsurface = _surfaces[in_arr[0].surfaceid()];
                Material *m = _materials[closestsurface->materialid()];
                Vector3 diff = m->diff();
                px.r = diff._a; 
                px.g = diff._b; 
                px.b = diff._c;
            } else {
                // If there is no intersection, set the current pixel to black
                px.r = px.g = px.b = 0;
            }
        }
    }
    this->writeRgba(filename, &res[0][0]);
}

void Camera::writeRgba(const char filename[], Rgba *pixels) {
    RgbaOutputFile file (filename, _pw, _ph, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, _pw);
    file.writePixels (_ph);
}


int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "useage: raytra scenefilename outputfilename" << endl;
        return -1;
    }

    Parser parser;
    Camera camera = parser.parse(argv[1]);
    camera.render(argv[2]);
}
