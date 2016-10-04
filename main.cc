#include <iostream>
#include <vector>
#include "surface.h"
#include "material.h"
#include "raytra.h"
#include "parse.h"

int main(int ac, char **av) {
    Parser parser;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
    Camera *camera = nullptr; 
    parser.parse(av[1], surfaces, materials, camera);
    std::cout << camera << std::endl;
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

