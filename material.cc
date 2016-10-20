/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#include "material.h"
#include <math.h>

using namespace std;


Vector3 Material::computeShading (
            const Vector3 &L, // unit vector to the light
            const Vector3 &e, // unit vector to the viewer
            const Vector3 &N, // unit surface normal
            const Vector3 &L_e // color vector of the light
            ) const 
{
    Vector3 res (0., 0., 0.);
    double n_dot_l = N.dotproduct(L);

    // Diffuse component computation
    if(n_dot_l > 0) {
        Vector3 diffvec (_diff._a * L_e._a * n_dot_l,
                         _diff._b * L_e._b * n_dot_l,
                         _diff._c * L_e._c * n_dot_l);
        res += diffvec;
    }

    // Specular component computation
    Vector3 half_vec = e;
    half_vec += L;

    // Check if the vector is towards the light
    if(half_vec.length() > 0.) { 

        half_vec.normalize();
        
        double n_dot_h = N.dotproduct(half_vec);

        if(n_dot_h > 0.) {
            n_dot_h = pow(n_dot_h, _r);
            Vector3 specvec (_spec._a * L_e._a * n_dot_h,
                             _spec._b * L_e._b * n_dot_h,
                             _spec._c * L_e._c * n_dot_h);

            res += specvec;
        }
    }
    
    return res;
}
