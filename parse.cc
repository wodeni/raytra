#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "raytra.h"
#include "point.h"

#define DEBUG 1
using namespace std;


void Parser::parse(const char *file,
        std::vector<Surface *> &surfaces,
        std::vector<Material *> &materials,
        Camera *&camera) {
    ifstream in(file);
    char buffer[1025];
    string cmd;

    // If no material is specified before an object,
    // we use the default material: all black
    Material *lastloadedmaterial = new Material(Vector3(), Vector3(), 0, Vector3());
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
#ifdef DEBUG
            cout << "got a sphere with ";
            cout << "Position: " << pos << " Radius: " << r << endl;
#endif
            surfaces.push_back(new Sphere(pos, r));
            materials.push_back(lastloadedmaterial);
#ifdef DEBUG
            cout << "llm: " << lastloadedmaterial->_diff << endl;
#endif
        } else if (cmd=="t") {
            Point a,b,c;
            iss >> a,b,c;
        } else if (cmd=="p") {
            Point n; float d;
            iss >> n >> d;
        } else if (cmd=="c") {
            Point pos; Vector3 dir; float d,iw,ih; int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            camera = new Camera(pos,dir,d,iw,ih,pw,ph);
#ifdef DEBUG
            cout << "got a camera with ";
            cout << "Position: " << pos << " Direction: " << dir << " Focal length: " << d  << " File: " << iw << "x" << ih <<  endl;
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
            lastloadedmaterial = new Material(diff, spec, r, refl);
        } else {
            cout << "Parser error: invalid command at line " << line << endl;
        }
    }
#ifdef DEBUG 
    for(int i = 0; i < materials.size(); ++i) 
        cout << materials[i]->_diff << endl;
    for(int i = 0; i < surfaces.size(); ++i) 
        cout << ((Sphere*) surfaces[i])->_origin << endl;
#endif
    in.close();
}
