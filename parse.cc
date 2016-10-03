#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

Sphere::Sphere(float3 origin, float radius)
    : origin(origin), radius(radius)
{}

Camera::Camera(float3 pos,
               float3 dir,
               float d, float iw, float ih,
               int pw, int ph)
    : pos(pos), dir(dir), d(d), iw(iw), ih(ih), pw(pw), ph(ph)
{}

Material::Material(float3 diff, float3 spec, float r, float3 refl) 
    : diff(diff), spec(spec), r(r), refl(refl)
{}
void Parser::parse(const char *file,
                     std::vector<Surface *> &surfaces,
                     std::vector<Material *> &materials,
                     Camera *camera) {
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
            float3 pos; float r;
            iss >> pos >> r;
            //sphere(pos,r);
            Surface *s = new Sphere(pos, r);
            surfaces.push_back(s);
        } else if (cmd=="t") {
            float3 a,b,c;
            iss >> a,b,c;
            //triangle(a,b,c);
        } else if (cmd=="p") {
            float3 n; float d;
            iss >> n >> d;
            //plane(n,d);
        } else if (cmd=="c") {
            float3 pos,dir; float d,iw,ih; int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            //camera(pos,dir,d,iw,ih,pw,ph);
            camera = new Camera(pos,dir,d,iw,ih,pw,ph);
        } else if (cmd=="l") {
            iss >> cmd;
            if (cmd=="p") {
                float3 pos,rgb;
                iss >> pos >> rgb;
                //pointLight(pos,rgb);
            } else if (cmd=="d") {
                float3 dir,rgb;
                iss >> dir >> rgb;
                //directionalLight(dir,rgb);
            } else if (cmd=="a") {
                float3 rgb;
                iss >> rgb;
                //ambientLight(rgb);
            } else {
                cout << "Parser error: invalid light at line " << line << endl;
            }
        } else if (cmd=="m") {
            float3 diff,spec,refl; float r;
            iss >> diff >> spec >> r >> refl;
            //material(diff,spec,r,refl);
            Material *m = new Material(diff, spec, r, refl);
            materials.push_back(m);
        } else {
            cout << "Parser error: invalid command at line " << line << endl;
        }
    }

    in.close();
}
