#ifndef PARSE_H
#define PARSE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "basemath.h"

using namespace std;


/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
inline istream &operator>>(istream &is, Point &f) {
    return is>>f._a>>f._b>>f._c;
}

inline istream &operator>>(istream &is, Vector3 &f) {
    return is>>f._a>>f._b>>f._c;
}

/* To avoid two header files including each other,
 * we just put the class name here to let the compiler
 * know Camera is a valid class without actually including
 * raytra.h, which includes this parse.h!
 */
class Camera; 

class Parser {
    public:
void parse(const char *file,
    std::vector< Surface * > &surfaces,
    std::vector< Material * > &materials,
    std::vector< P_Light * > &lights,
    Camera &cam); 
};


#endif /* PARSE_H */
