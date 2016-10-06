#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "raytra.h"
#include "parse.h"

using namespace std;
using namespace Imf;

int main(int argc, char **argv) {

    if (argc != 3) {
        cerr << "useage: raytra scenefilename outputfilename" << endl;
        return -1;
    }

    Parser parser;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
    Camera *camera = nullptr; 
    parser.parse(argv[1], surfaces, materials, camera);
    cout << materials[0]->_diff << endl;
    Array2D<Rgba> pic;
    camera->render(pic, surfaces, materials);
    camera->writeRgba(argv[2], &pic[0][0]);

    // Testing...
    /*
    Vector3 v1(9, 1, 0);
    Vector3 v2(1, 1, 0);
    Vector3 v3 = v1.crossproduct(v2);
    double tmp = v1.dotproduct(v2);
    cout << v3 << " " <<  tmp << endl;
    std::cout << *camera << std::endl;
    std::cout << *(Sphere *)surfaces[0] << std::endl;
    Ray r1 = camera->construct_ray(0, 0);
    cout << r1._dir << " " << r1._origin << endl; 
    */
    //Point p(0, 13.67, -62.57);
    //Ray r2(camera->_eye, (camera->_eye - p));
    //Sphere s(p, 10.0);
    //Intersection in = s.intersect(r2);
    //cout << in.intersected << endl; 
    
    // Deallocate used memory
    for(int i = 0; i < surfaces.size(); ++i) {
        delete surfaces[i];
    }
    surfaces.clear();

    for(int i = 0; i < materials.size(); ++i) {
        delete materials[i];
    }
    materials.clear();
    delete camera;

}

