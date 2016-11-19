/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef PARSE_H
#define PARSE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "basemath.h"
#include "light.h"
#include "constants.h"

using namespace std;

inline istream &operator>>(istream &is, Point &f) {
    return is>>f._xyz[0]>>f._xyz[1]>>f._xyz[2];
}

inline istream &operator>>(istream &is, Vector3 &f) {
    return is>>f._xyz[0]>>f._xyz[1]>>f._xyz[2];
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
    std::vector< Light * > &lights,
	std::vector<Vector3> &normals,
    Camera &cam); 
	void read_wavefront_file(const char *file, std::vector<int> &tris,
				std::vector<float> &verts);
//	void read_wavefront_file(const char *file, std::vector<Surface *> &triangles);
};




#endif /* PARSE_H */
