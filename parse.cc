#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "raytra.h"
#include "sphere.h"
#include "point.h"

using namespace std;


void Parser::parse(const char *file,
                     std::vector<Surface *> &surfaces,
                     std::vector<Material *> &materials,
                     Camera *&camera) {
    ifstream in(file);
    char buffer[1025];
    string cmd;

    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;

        // If no material is specified before an object,
        // we use the default material: all black
        Material *lastloadedmaterial = new Material(Vector3(), Vector3(), 0, Vector3());

        cmd="";

        istringstream iss(buffer);

        iss >> cmd;
        if (cmd[0]=='/' or cmd.empty()) {
            continue;
        } else if (cmd=="s") {
            Point pos; float r;
            iss >> pos >> r;
#ifdef IM_DEBUGGING
            cout << "got a sphere with ";
            cout << "Position: " << pos << " Radius: " << r << endl;
#endif
            surfaces.push_back(new Sphere(pos, r));
            materials.push_back(lastloadedmaterial);
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
            Material *m = new Material(diff, spec, r, refl);
            lastloadedmaterial = m;
        } else {
            cout << "Parser error: invalid command at line " << line << endl;
        }
    }

    in.close();
}
