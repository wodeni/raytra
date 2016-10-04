#ifndef MATERIAL_H
#define MATERIAL_H
#include "vector3.h"

class Material {
    public:
        Vector3 _diff; // Diffused RGB value
        Vector3 _spec; // Specular RGB value
        float _r; // Roughness
        Vector3 _refl; // Ideal specular RGB calue
        Material(Vector3 _diff, Vector3 _spec, float _r, Vector3 _refl);
};


#endif /* MATERIAL_H */
