#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "raytra.h"
#include "parse.h"

using namespace std;

int main(int ac, char **av) {

    if (argc != 2) {
        cerr << "useage: raytra scenefilename" << endl;
        return -1;
    }

    Parser parser;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
    Camera *camera = nullptr; 
    parser.parse(av[1], surfaces, materials, camera);

    // Testing...
    Vector3 v1(0, 1, 0);
    Vector3 v2(1, 0, 0);
    Vector3 v3 = v1.crossproduct(v2);
    cout << v3 << endl;
    std::cout << *camera << std::endl;
    std::cout << surfaces.size() << " " << materials.size() << " " << camera->_pw << " " << camera->_ph << " " << std::endl;

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

