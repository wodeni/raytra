#include "parse.h"
#include "raytra.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#define DEBUG 0 
using namespace Imf;
using namespace std;

void Camera::initCamera (Point &pos, Vector3 &dir,
        float &d, float &iw, float &ih,
        int &pw, int &ph)
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
#if DEBUG 
    cout << *this ;
#endif
}

Camera::~Camera() {
#if DEBUG
    cout << "Destructing Camera..." << endl;
#endif
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

void Camera::render(const char filename[],
                std::vector< Surface *> &surfaces,
                std::vector< Material *> &materials,
                std::vector< P_Light * > &lights) {

    Array2D<Rgba> res;
    res.resizeErase(_ph, _pw);

    for(int i = 0; i < _pw; ++i) { 
        for(int j = 0; j < _ph; ++j) {
            // Construct ray
            Ray r = construct_ray(i,j); 
            std::vector<Intersection> in_arr;
            Rgba& px = res[_ph - 1 - j][i]; // the current px in the picture

            // Intersect the scene
            for(std::size_t k = 0; k < surfaces.size(); ++k) {
                Intersection tmp;
                tmp = surfaces[k]->intersect(r);
                if(tmp.Intersected()) {
                    tmp.setsurfaceid(k);
                    in_arr.push_back(tmp);
                }
            }  

            if(in_arr.size() != 0) {
                // From the result of intersecting, color the pixel
                std::sort(in_arr.begin(), in_arr.end());
                // Pick the closest object 
                Intersection closest_in = in_arr[0];
                Surface* closestsurface = surfaces[closest_in.surfaceid()];
                Material *m = materials[closestsurface->materialid()];
                // Compute the pixel intensity
                // Version #1: get only the diffused color of the material, no shading
                    // Vector3 diff = m->diff();
                    // px.r = diff._a; 
                    // px.g = diff._b; 
                    // px.b = diff._c;
                // Diffuse shading computation
                    Vector3 diff = m->diff();
                    Vector3 spec = m->spec();
                    float phong = m->phong();
                    P_Light *light = lights[0]; // At this stage we have only one light
                    double distance = (closest_in.getIntersectionPoint() - Point(0,0,0)).length(); 
                    double d2 = 1. / (distance * distance);
                    Vector3 l = light->getPosition() - closest_in.getIntersectionPoint();
                    l = l.normalize();
                    Vector3 n = closest_in.getNormal();
                    double ndotl = n.dotproduct(l);
                    Vector3 l_color = light->getColor();
                    Vector3 ld;
                    ld._a = diff._a * l_color._a * d2 * std::max(0., ndotl); 
                    ld._b = diff._b * l_color._b * d2 * std::max(0., ndotl); 
                    ld._c = diff._c * l_color._c * d2 * std::max(0., ndotl);
               // Soecular shading computation
                    Vector3 v = (-1.0 * r._dir).normalize();
                    Vector3 h = (v + l).normalize();
                    double ndoth = n.dotproduct(h);
                    Vector3 ls;
                    ls._a = spec._a * l_color._a * d2 * pow(std::max(0., ndoth), phong); 
                    ls._b = spec._b * l_color._b * d2 * pow(std::max(0., ndoth), phong);
                    ls._c = spec._c * l_color._c * d2 * pow(std::max(0., ndoth), phong);

                    px.r = ld._a + ls._a;
                    px.g = ld._b + ls._b;
                    px.b = ld._c + ls._c;
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

    Camera cam;
    std::vector< P_Light * > lights;
    std::vector< Surface * > surfaces;
    std::vector< Material * > materials;

    // If no material is specified before an object, we use the default material: all black
    Material *defaultMaterial = new Material();
    materials.push_back(defaultMaterial);
    Parser parser;
    parser.parse(argv[1], surfaces, materials, lights, cam);
    cam.render(argv[2], surfaces, materials, lights);

    // Dealloctating the memory
    for(Material* m : materials) {
        delete m;
    }
    materials.clear();
    for(Surface* s : surfaces) {
        delete s;
    }
    surfaces.clear();
    for(P_Light* l : lights) {
        delete l;
    }
    lights.clear();
}
