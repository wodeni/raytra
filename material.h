/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef MATERIAL_H
#define MATERIAL_H
#include "basemath.h"

/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
class Material {
    friend std::ostream &operator<<(const std::ostream &, const Material);
    public:
        Material() {
            _diff = Vector3(0, 0, 0);
            _spec = Vector3(0, 0, 0);
            _refl = Vector3(0, 0, 0);
            _r = 0.0;
        }

        Material(const Vector3 &diff, const Vector3 &spec, const double &r, const Vector3 &refl)
    : _diff(diff), _spec(spec), _r(r), _refl(refl) {}

        Vector3 diff() const { return _diff; }

        Vector3 spec() const { return _spec; }

        double phong() const { return _r; }

        Vector3 computeShading (
                    const Vector3 &L, // unit vector to the light
                    const Vector3 &e, // unit vector to the viewer
                    const Vector3 &N, // unit surface normal
                    const Vector3 &L_e // color vector of the light
                    ) const;

    private:
        Vector3 _diff; // Diffused RGB value
        Vector3 _spec; // Specular RGB value
        double _r; // Roughness
        Vector3 _refl; // Ideal specular RGB calue
};


#endif /* MATERIAL_H */
