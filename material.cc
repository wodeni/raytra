#include "material.h"

Material::Material(Vector3 diff, Vector3 spec, float r, Vector3 refl) 
    : _diff(diff), _spec(spec), _r(r), _refl(refl)
{}
