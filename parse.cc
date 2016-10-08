/**
 * @author: Wode "Nimo" Ni
 * @version: 2016/10/07
 */
#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>

#include "raytra.h" // Do the actual include here

#define DEBUG 0

using namespace std;


Camera& Parser::parse(const char *file) {
    ifstream in(file);
    char buffer[1025];
    string cmd;

    int cameracount = 0;
    int surfacecount = 0;
    Surface* cursurface = nullptr;

    Camera *camera = nullptr;

    // TODO: are vectors dynamically allocated internally?
    // Surface and material vectors, to be stored by the camera
    std::vector<Material *> materials;
    std::vector<Surface *> surfaces;
    
    // If no material is specified before an object, we use the default material: all black
    Material *curmaterial = new Material();
    materials.push_back(curmaterial);

    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;

        cmd="";

        istringstream iss(buffer);

        iss >> cmd;
        if (cmd[0]=='/' or cmd.empty()) {
            continue;
        } else if (cmd=="s") {
            Point pos; float r;
            iss >> pos >> r;
            cursurface = new Sphere(pos, r);
            surfacecount++;
#if DEBUG
            cout << "got a sphere with ";
            cout << "Position: " << pos << " Radius: " << r << endl;
#endif
        } else if (cmd=="t") {
            // triangle
            Point a,b,c;
            iss >> a >> b >> c;
        } else if (cmd=="p") {
            // Plane
            Point n; float d;
            iss >> n >> d;
        } else if (cmd=="c") {
            // Camera
            Point pos; Vector3 dir; float d,iw,ih; int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            camera = new Camera();
            camera->initCamera(pos,dir,d,iw,ih,pw,ph,materials,surfaces);
            cameracount++;
#if DEBUG
            cout << "got a camera with ";
            cout << "Position: " << pos << " Direction: " << dir << " Focal length: " << d  << " Film size: " << iw << "x" << ih <<  endl;
            cout << "Output size: " << pw << "x" << ph << endl;
#endif
        } else if (cmd=="l") {
            iss >> cmd;
            if (cmd=="p") {
                Point pos; Vector3 rgb;
                iss >> pos >> rgb;
            } else if (cmd=="d") {
                Vector3 dir,rgb;
                iss >> dir >> rgb;
            } else if (cmd=="a") {
                Vector3 rgb;
                iss >> rgb;
            } else {
                cout << "Parser error: invalid light at line " << line << endl;
            }
        } else if (cmd=="m") {
            Vector3 diff,spec,refl; float r;
            iss >> diff >> spec >> r >> refl;
            curmaterial = new Material(diff, spec, r, refl);
            materials.push_back(curmaterial);
        } else {
            cout << "Parser error: invalid command at line " << line << endl;
        }
        

        if(cursurface != nullptr) {
            cursurface->setmaterialid(materials.size()-1);
            surfaces.push_back(cursurface);
#if DEBUG
            cout << "Constructed a surface with material# " << cursurface->materialid() << endl;
#endif
            cursurface = nullptr;
        }
    }
    in.close();
    // Make sure we have some surfaces to render
    assert(surfaces.size() != 0);
    // As mentioned in class #5, we allow only one camera in the scene
    assert(cameracount == 1);
    camera->setmaterials(materials);
    camera->setsurfaces(surfaces);
#if DEBUG 
    cout << "Summary: " << surfacecount << " surfaces scanned " << materials.size() << " materials scanned" << endl;
#endif
    return *camera;
}
