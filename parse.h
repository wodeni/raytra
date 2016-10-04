#ifndef PARSE_H
#define PARSE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "point.h"
#include "raytra.h"
#include "vector3.h"

using namespace std;


inline istream &operator>>(istream &is, Point &f) {
    return is>>f._a>>f._b>>f._c;
}

inline istream &operator>>(istream &is, Vector3 &f) {
    return is>>f._a>>f._b>>f._c;
}

class Parser {
    public:
        virtual void parse(const char *file,
                std::vector<Surface *> &surfaces,
                std::vector<Material *> &materials,
                Camera *&camera);
};


#endif /* PARSE_H */
