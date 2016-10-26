#include "parse.h"
#include "raytra.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
using namespace Imf;
using namespace std;

void Camera::initCamera (Point &pos, Vector3 &dir,
        double &d, double &iw, double &ih,
        int &pw, int &ph)
{
    _eye = pos;
    _d = d;
    _iw = iw; _ih = ih;
    _pw = pw; _ph = ph;
    Vector3 up(0, 1, 0);
    _u = dir.crossproduct(up);
    _v = _u.crossproduct(dir);
    _w = (dir * -1.0);
    _u.normalize();  _v.normalize(); _w.normalize();
}

Camera::~Camera() {

}

std::ostream &operator<<(std::ostream &os, const Camera &c) {
    os << "Camera frame:" << c._u  << " " << c._v  << " " << c._w << std::endl;
    return os;
}

/* Construct a ray given the (i,j)th pixel */
Ray Camera::construct_ray (int i, int j) {
    // Compute the coordinates of the pixel center
    double r, l, t, b, u, v;
    Vector3 dir;
    r = _iw / 2.0;
    l = -1.0 * r;
    t = _ih / 2.0;
    b = -1.0 * t;
    u = l + _iw * (i + 0.5) / _pw;
    v = b + _ih * (j + 0.5) / _ph;

    dir = (u * _u + v * _v + (-1 * _d) * (_w));
    dir.normalize();
    return Ray(_eye, dir);
}

void Camera::render(const char filename[],
                std::vector< Surface *> &surfaces,
                std::vector< Material *> &materials,
                std::vector< Light * > &lights) {

	std::cout << "Rendering";

	int printProgress = _pw * _ph / 10;

    Array2D<Rgba> res;
    res.resizeErase(_ph, _pw);

    for(int i = 0; i < _pw; ++i) { 
        for(int j = 0; j < _ph; ++j) {

        	if(printProgress && (i * _ph + j) % printProgress == 0) {
        		std::cout << ".";
        		std::cout.flush();
        	}

            Rgba& px = res[_ph - 1 - j][i]; // the current px in the picture
            
            // Construct ray
            Ray r = construct_ray(i,j); 
            std::vector<Intersection> in_arr;

            // Intersect the scene
            for(std::size_t k = 0; k < surfaces.size(); ++k) {

                Intersection tmp;
                Surface* s = surfaces[k];
                tmp = s->intersect(r);

                if(tmp.Intersected()) {
                    tmp.setsurfaceid(k);
                    in_arr.push_back(tmp);
                }
            }  

            if(in_arr.size() != 0) {

                // From the result of intersecting, color the pixel
                std::sort(in_arr.begin(), in_arr.end());

                // Pick the closest object 
                Intersection closest_in = in_arr[0];
                Surface* closestsurface = surfaces[closest_in.surfaceid()];
                Material *m = materials[closestsurface->materialid()];

                Point intersection = closest_in.getIntersectionPoint();
				Vector3 N = closest_in.getNormal();
				Vector3 e = -1.0 * r._dir; // Direction of the ray already normalized

				Vector3 rgb (0., 0., 0.);
                for(Light *light : lights) {

                	// If the light is ambient, only add the piecewise multiply of the ambient light's power
                	// times the material's diffuse color (Kd).
                	if(light->isAmbient()) {
                		rgb += m->diff().pieceMultiply(light->getColor());
                		continue;
                	}

					Vector3 L = light->getPosition() - intersection;
					double d2 = L.dotproduct(L);
					L.normalize();
					Vector3 L_e = light->getColor();

					// Generate shadow ray fron hit point to the light source
					bool blocked = false;
					Ray shadow_ray (intersection, L);
					for(Surface* s : surfaces) {
						Intersection temp = s->intersect(shadow_ray);
						double t = temp.getT();
						// TODO: to save a sqrt, squared both sides. Is this correct?
						if(temp.Intersected() && temp.getNormal().dotproduct(L) < 0. && t * t < d2) {
							blocked = true;
							break;
						}
					}

					// If the light source is not blocked by other objects,
					// compute shading and scale by 1 over the distance to the light squared
					if(!blocked)
						rgb += m->computeShading(L, e, N, L_e) * (1.0 / d2);
                }

				px.r = rgb._a;
				px.g = rgb._b;
				px.b = rgb._c;
                    
            } else {
                // If there is no intersection, set the current pixel to black
                px.r = px.g = px.b = 0;
            }
        }
    }
    this->writeRgba(filename, &res[0][0]);
    std::cout << "done!" << std::endl;
}

void Camera::writeRgba(const char filename[], Rgba *pixels) {
    RgbaOutputFile file (filename, _pw, _ph, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, _pw);
    file.writePixels (_ph);
}


int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "useage: raytra scenefilename outputfilename" << endl;
        return -1;
    }

    Camera cam;
    std::vector< Light * > lights;
    std::vector< Surface * > surfaces;
    std::vector< Material * > materials;

    // If no material is specified before an object, we use the default material: all black
    Material *defaultMaterial = new Material();
    materials.push_back(defaultMaterial);

    Parser parser;
    parser.parse(argv[1], surfaces, materials, lights, cam);


    Vector3 lightcolor (1., 1., 1.);
    if(lights.size() == 0) {
		Light *defaultLight = new PointLight(cam.getPosition(), lightcolor);
		lights.push_back(defaultLight);
		std::cout << "No lights! Using default light instead" << std::endl;
    }
	cam.render(argv[2], surfaces, materials, lights);

    // Dealloctating the memory
    for(Material* m : materials) {
        delete m;
    }
    materials.clear();
    for(Surface* s : surfaces) {
        delete s;
    }
    surfaces.clear();
    for(Light* l : lights) {
        delete l;
    }
    lights.clear();
}
