#ifndef PRP_H
#define PRP_H

#include "parse.h"

class POVRayParser : public Parser {
    string oStr,mStr,aStr;

    virtual void sphere(float3 pos, float r);
    virtual void triangle(float3 p1, float3 p2, float3 p3);
    virtual void plane(float3 n, float d);
    virtual void camera(float3 pos, float3 dir, float d, float iw, float ih, int pw, int ph);
    virtual void pointLight(float3 pos, float3 rgb);
    virtual void directionalLight(float3 dir, float3 rgb);
    virtual void ambientLight(float3 rgb);
    virtual void material(float3 diff, float3 spec, float r, float3 refl);

    void finalizeObject();

    public:
        void parse(const char *file,
                    std::vector<Surface *> &surfaces,
                    std::vector<Material *> &materials,
                    Camera *&camera); 
};

#endif
