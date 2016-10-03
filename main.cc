#include "prp.h"

int main(int ac, char **av) {
    POVRayParser parser;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
    Camera *camera = nullptr; 
    parser.parse(av[1], surfaces, materials, camera);
    std::cout << camera << std::endl;
    std::cout << surfaces.size() << " " << materials.size() << " " << camera->pw << " " << camera->ph << " " << std::endl;
}
