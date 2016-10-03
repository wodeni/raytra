#include "prp.h"

int main(int ac, char **av) {
    POVRayParser parser;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
    Camera *camera; 
    parser.parse(av[1], surfaces, materials, camera);
    std::cout << surfaces.size() << " " << materials.size() << std::endl;
}
