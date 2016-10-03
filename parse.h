#ifndef PARSE_H
#define PARSE_H

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct float3 {
    float x,y,z;
};

inline istream &operator>>(istream &is, float3 &f) {
    return is>>f.x>>f.y>>f.z;
}
inline ostream &operator<<(ostream &os, float3 &f) {
    return os<<"<"<<f.x<<","<<f.y<<","<<f.z<<">";
}

/**
 * Superclass for objects in the scene
 */
class Surface {

};

class Sphere : public Surface {
    public:
        float3 origin;
        float radius;
        Sphere(float3 origin, float radius);
};

class Camera {
    public:
        float3 pos, dir;
        float d, iw, ih;
        int pw,ph;
        Camera(float3 pos,
               float3 dir,
               float d, float iw, float ih,
               int pw, int ph);
};

class Material {
    public:
        float3 diff; // Diffused RGB value
        float3 spec; // Specular RGB value
        float r; // Roughness
        float3 refl; // Ideal specular RGB calue
        Material(float3 diff, float3 spec, float r, float3 refl);
};
class Parser {
    protected:
        virtual void sphere(float3 pos, float r) {}
        virtual void triangle(float3 p1, float3 p2, float3 p3) {}
        virtual void plane(float3 n, float d) {}
        virtual void camera(float3 pos, float3 dir, float d, float iw, float ih, int pw, int ph) {}
        virtual void pointLight(float3 pos, float3 rgb) {}
        virtual void directionalLight(float3 dir, float3 rgb) {}
        virtual void ambientLight(float3 rgb) {}
        virtual void material(float3 diff, float3 spec, float r, float3 refl) {}

    public:
        virtual void parse(const char *file,
                std::vector<Surface *> &surfaces,
                std::vector<Material *> &materials,
                Camera *camera);
};


#endif
