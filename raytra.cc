#include "parse.h"
#include "raytra.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace Imf;
using namespace std;

#define SHADOW_RAY 0
#define REGULAR_RAY 1
#define REFLECT_RAY 2
#define STEP_NUM 0.0001

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

    // The yellow material used to debug is pushed to the tail of the vector
    Material* yellow = new Material(Vector3(1, 1, 0), Vector3(0, 0, 0), 0., Vector3(0, 0, 0));
    materials.push_back(yellow);

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
            Vector3 rgb = L	(r,
					 20,
					 0.001,std::numeric_limits<double>::max(),
					 REGULAR_RAY,
					 surfaces, materials, lights);
            px.r = rgb._a; px.g = rgb._b; px.b = rgb._c;
        }
    }
    this->writeRgba(filename, &res[0][0]);
    std::cout << "done!" << std::endl;
}

Vector3 Camera::L(Ray& r,
		int recursive_limit,
		double min_t, double max_t,
		int ray_type,
		std::vector< Surface *> &surfaces,
        std::vector< Material *> &materials,
		std::vector< Light *> &lights) {


	if(recursive_limit == 0) return Vector3 (0, 0, 0);

	if(ray_type == SHADOW_RAY) {
		for(Surface* s : surfaces) {
			Intersection in;
			if(s->intersect(r, in) && (in.getT() > STEP_NUM && in.getT() < max_t)) {
				return Vector3 (0, 0, 0);
			}
		}
		// TODO: any non-zero value is good?
		return Vector3(1, 1, 1);
	}

	double best_t = std::numeric_limits<double>::max();
	Intersection best_in, tmp;
	int m_id;

	// Intersect the scene
	for(Surface *obj : surfaces) {
		if(obj->intersect(r, tmp)) {
			double t = tmp.getT();
			if(t > min_t && t < best_t && t < max_t) {
				best_t = t;
				best_in =tmp;
				m_id = obj->materialid();
			}
		}
	}
//	for(std::size_t k = 0; k < surfaces.size(); ++k) {
//		Surface* s = surfaces[k];
//		tmp = s->intersect(r);
//		double t = tmp.getT();
//		if(tmp.Intersected() && t > min_t && t <  best_t && t < max_t) {
//			foundIntersection = true;
//			tmp.setsurfaceid(k);
//			best_in = tmp;
//			best_t = t;
//			m = materials[s->materialid()];
//			intersection = tmp.getIntersectionPoint();
//			N = tmp.getNormal();
//		}
//	}

	if(best_in.Intersected()) {

		Material *m = materials[m_id];
		Vector3 N = best_in.getNormal();
		Point intersection = best_in.getIntersectionPoint();

		Vector3 e = -1.0 * r._dir; // Direction of the ray already normalized

		// If the camera is pointing to the back of the surface, color it yellow
		if(e.dotproduct(N) < 0.) {
			m = materials.back();
			N = -1.0 * N;
		}

		Vector3 rgb (0., 0., 0.);
		for(Light *light : lights) {

			// If the light is ambient, only add the piecewise multiply of the ambient light's power
			// times the material's diffuse color (Kd).
			if(light->isAmbient() && ray_type == REGULAR_RAY) {
				rgb += m->diff().pieceMultiply(light->getColor());
				continue;
			}

			Vector3 l = light->getPosition() - intersection;
			double d_length = l.length();
			double d2 = d_length * d_length;
			l.normalize();
			Vector3 L_e = light->getColor();

			// Generate shadow ray fron hit point to the light source
			Ray shadow_ray (intersection, l);
			Vector3 l_rgb = L(shadow_ray,
			  1,
			  STEP_NUM, d_length,
			  SHADOW_RAY,
			  surfaces, materials, lights);
			if(l_rgb.length() != 0.) {
				rgb += m->computeShading(l, e, N, L_e) * (1.0 / d2);
			}
		}

		if(!(m->reflective())) {
			return rgb;
		}
		else {
			double d_dot_N = r._dir.dotproduct(N);
			Vector3 refl_dir = r._dir - (2 * d_dot_N) * N;
			Ray refl_r (intersection, refl_dir);
			Vector3 refl_rgb = 	L(refl_r,
								  recursive_limit - 1,
								  STEP_NUM, std::numeric_limits<double>::max(),
								  REFLECT_RAY,
								  surfaces, materials, lights);
			return rgb + m->refl().pieceMultiply(refl_rgb);
		}

	} else {
		// If there is no intersection, set the current pixel to black
		return Vector3 (0, 0 ,0);
	}
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
