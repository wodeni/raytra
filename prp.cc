#include "prp.h"

#include <cmath>
#include <sstream>

void POVRayParser::sphere(float3 pos, float r) {
    finalizeObject();
    ostringstream os;
    os << "sphere {" << endl;
    os << "  " << pos << ", " << r << endl;
    oStr = os.str();
}

void POVRayParser::triangle(float3 p1, float3 p2, float3 p3) {
    finalizeObject();
    ostringstream os;
    os << "triangle {" << endl;
    os << "  " << p1 << ", " << p2 << ", " << p3 << endl;
    oStr = os.str();
}

void POVRayParser::plane(float3 n, float d) {
    finalizeObject();
    ostringstream os;
    os << "plane {" << endl;
    os << "  " << n << ", " << d << endl;
    oStr = os.str();
}

void POVRayParser::camera(float3 pos, float3 dir, float d, float iw, float ih, int pw, int ph) {
    cout << "camera {" << endl;
    cout << "  location " << pos << endl;
    cout << "  right <" << iw << ",0,0>" << endl;
    cout << "  up <0," << ih << ",0>" << endl;
    cout << "  direction <0,0," << -d << ">" << endl;
    dir.x += pos.x;
    dir.y += pos.y;
    dir.z += pos.z;
    cout << "  look_at " << dir << endl;
    cout << "}" << endl;
}

void POVRayParser::pointLight(float3 pos, float3 rgb) {
    cout << "light_source {" << endl;
    cout << "  " << pos << endl;
    cout << "  color rgb " << rgb << endl;
    cout << "}" << endl;
}

void POVRayParser::directionalLight(float3 dir, float3 rgb) {
    // povray has no directional lights, so
    // approximate with a faraway point light
    dir.x*=(100000.0);
    dir.y*=(100000.0);
    dir.z*=(100000.0);
    cout << "light_source {" << endl;
    cout << "  " << dir << endl;
    cout << "  color rgb " << rgb << endl;
    cout << "}" << endl;
}

void POVRayParser::ambientLight(float3 rgb) {
    ostringstream os;
    os << "global_settings { ambient_light rgb " << rgb << "}" << endl;
    aStr = os.str();
}

void POVRayParser::material(float3 diff, float3 spec, float r, float3 refl) {
    ostringstream os;
    os << "  pigment { color rgb " << diff << " }" << endl;
    os << "  finish {" << endl;
    // povray doesn't take reflective color, so just approximate a blend
    // weight:
    float dlen = sqrt(refl.x*refl.x+refl.y*refl.y+refl.z*refl.z);
    os << "    reflection " << dlen << endl;
    os << "    phong " << r << endl;
    os << "    ambient 1.0" << endl;
    os << "  }" << endl;
    mStr = os.str();  
}

void POVRayParser::finalizeObject() {
    if (!oStr.empty()) {
        cout << oStr << mStr;
        cout << "}" << endl;
    }
    mStr.clear(); oStr.clear();
}

void POVRayParser::parse(const char *file,
        std::vector<Surface *> &surfaces,
        std::vector<Material *> &materials,
        Camera *&camera) {
    Parser::parse(file, surfaces, materials, camera);
    finalizeObject();
    if (aStr.empty()) {
        aStr = "global_settings { ambient_light rgb <0,0,0> }\n";
    }
    cout << aStr;
}
