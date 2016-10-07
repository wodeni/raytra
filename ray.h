#ifndef RAY_H
#define RAY_H

#include "basemath.h"

/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
class Ray {
    public:
        Point _origin;
        Vector3 _dir;
        Ray (const Point &origin, const Vector3 &dir) 
            : _origin(origin), _dir(dir) {}
};

#endif /* RAY_H */
