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

        cmd="";

        istringstream iss(buffer);

        iss >> cmd;
        if (cmd[0]=='/' or cmd.empty()) {
            continue;
        } else if (cmd=="s") {
            Point pos; float r;
            iss >> pos >> r;
            //sphere(pos,r);
            surfaces.push_back(new Sphere(pos, r));
        } else if (cmd=="t") {
            Point a,b,c;
            iss >> a,b,c;
            //triangle(a,b,c);
        } else if (cmd=="p") {
            Point n; float d;
            iss >> n >> d;
            //plane(n,d);
        } else if (cmd=="c") {
            Point pos; Vector3 dir; float d,iw,ih; int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            //camera(pos,dir,d,iw,ih,pw,ph);
            camera = new Camera(pos,dir,d,iw,ih,pw,ph);
        } else if (cmd=="l") {
            iss >> cmd;
            if (cmd=="p") {
                Point pos; Vector3 rgb;
                iss >> pos >> rgb;
                //pointLight(pos,rgb);
            } else if (cmd=="d") {
                Vector3 dir,rgb;
                iss >> dir >> rgb;
                //directionalLight(dir,rgb);
            } else if (cmd=="a") {
                Vector3 rgb;
                iss >> rgb;
                //ambientLight(rgb);
            } else {
                cout << "Parser error: invalid light at line " << line << endl;
            }
        } else if (cmd=="m") {
            Vector3 diff,spec,refl; float r;
            iss >> diff >> spec >> r >> refl;
            //material(diff,spec,r,refl);
            Material *m = new Material(diff, spec, r, refl);
            materials.push_back(new Material(diff, spec, r, refl));
        } else {
            cout << "Parser error: invalid command at line " << line << endl;
        }
    }

    in.close();
}
