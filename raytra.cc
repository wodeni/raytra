#include "parse.h"
#include "raytra.h"
#include "surface.h"
#include "constants.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <cassert>
#include <cstdlib>

using namespace Imf;
using namespace std;

int mode = 0;
int CAMSAMPLES = 0;
int SHADOWSAMPLES = 0;
int YELLOW_INDEX = -1;

void Camera::initCamera(Point &pos, Vector3 &dir, double &d, double &iw,
		double &ih, int &pw, int &ph) {
	_eye = pos;
	_d = d;
	_iw = iw;
	_ih = ih;
	_pw = pw;
	_ph = ph;
	Vector3 up(0, 1, 0);
	_u = dir.crossproduct(up);
	_v = _u.crossproduct(dir);
	_w = (dir * -1.0);
	_u.normalize();
	_v.normalize();
	_w.normalize();
}

Camera::~Camera() {

}

std::ostream &operator<<(std::ostream &os, const Camera &c) {
	os << "Camera frame:" << c._u << " " << c._v << " " << c._w << std::endl;
	return os;
}

/* Construct a ray given the (i,j)th pixel */
Ray Camera::construct_ray_center(double i, double j) {
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

Ray Camera::construct_ray(double i, double j, double x_offset, double y_offset) {
	double r, l, t, b, u, v;
	Vector3 dir;
	r = _iw / 2.0;
	l = -1.0 * r;
	t = _ih / 2.0;
	b = -1.0 * t;
	u = l + _iw * (i + x_offset) / _pw;
	v = b + _ih * (j + y_offset) / _ph;
	dir = (u * _u + v * _v + (-1 * _d) * (_w));
	dir.normalize();
	return Ray(_eye, dir);
}


void Camera::render(const char filename[], std::vector<Surface *> &surfaces,
		std::vector<Material *> &materials, std::vector<Light *> &lights, BBoxNode *root) {

	std::cout << "Rendering";
	int printProgress = _pw * _ph / 10;
	srand(1); // Seed the random generator, as required by hw1.5
	Array2D<Rgba> res;
	res.resizeErase(_ph, _pw);

	for (int i = 0; i < _pw; ++i) {
		for (int j = 0; j < _ph; ++j) {

			if (printProgress && (i * _ph + j) % printProgress == 0) {
				std::cout << ".";
				std::cout.flush();
			}

			Rgba& px = res[_ph - 1 - j][i]; // the current px in the picture

			Vector3 rgb(0, 0, 0);

			for(int p = 0; p < CAMSAMPLES; p++) {
				for(int q = 0; q < CAMSAMPLES; q++) {

					// generate two random numbers between 0 and 1
					double x_offset =  (p + ((double)rand() / RAND_MAX)) / CAMSAMPLES;
					double y_offset =  (q + ((double)rand() / RAND_MAX)) / CAMSAMPLES;

					// Construct ray
					Ray r = construct_ray(i, j, x_offset, y_offset);
					rgb += L(r, RECURSION_MAX, STEP_NUM, DOUBLE_MAX, REGULAR_RAY, surfaces, materials, lights, root) *
							(1 / (double)(CAMSAMPLES * CAMSAMPLES));
				}
			}
			px.r = rgb._xyz[0];
			px.g = rgb._xyz[1];
			px.b = rgb._xyz[2];
            px.a = 1; // The alpha value must be 1 for everything to be visible
		}
	}
	this->writeRgba(filename, &res[0][0]);
	std::cout << "done!" << std::endl;
}

Vector3 Camera::L(Ray& r, int recursive_limit, double min_t, double max_t,
		int ray_type, std::vector<Surface *> &surfaces,
		std::vector<Material *> &materials, std::vector<Light *> &lights,
		BBoxNode *root) {
	// Base case
	if (recursive_limit == 0)
		return Vector3(0, 0, 0);

	// Shadow ray
	if (ray_type == SHADOW_RAY) {
		Intersection in;
		double best_t = DOUBLE_MAX;
		if (root->checkshadow(r, in, best_t)
				&& (in.getT() > STEP_NUM && in.getT() < max_t)) {
			return Vector3(0, 0, 0);
		} else {
			return Vector3(1, 1, 1);
		}
	}

	double best_t = DOUBLE_MAX;
	Intersection best_in;
//	int m_id = 0;

	// Intersect the scene

	root->intersect(r, best_in, best_t);

	if (best_in.Intersected()) {

//		Material *m = materials[m_id];
		Material *m = materials[best_in.getMaterialId()];
		Vector3 N = best_in.getNormal();
		Point intersection = best_in.getIntersectionPoint();

#if VERBOSE
		if(best_in.getNormal().length() == 0)
			cout << "a normal with length zero!" << endl;
#endif

		Vector3 e = -1.0 * r._dir; // Direction of the ray already normalized

		Vector3 rgb(0., 0., 0.);
		for (Light *light : lights) {

			Vector3 L_e = light->getColor();

			// If the light is ambient, only add the piecewise multiply of the ambient light's power
			// times the material's diffuse color (Kd).
			if (light->isAmbient() && ray_type == REGULAR_RAY) {
				rgb += m->diff().pieceMultiply(L_e);
				continue;
			}

			if(light->isArea()) {
				AreaLight *light_area = (AreaLight *)light;
				for(int p = 0; p < SHADOWSAMPLES; p++) {
					for(int q = 0; q < SHADOWSAMPLES; q++) {
						double x_offset =  (p + ((double)rand() / RAND_MAX)) / SHADOWSAMPLES;
						double y_offset =  (q + ((double)rand() / RAND_MAX)) / SHADOWSAMPLES;
						Vector3 l = light_area->createSample(x_offset, y_offset) - intersection;
						double d_length = l.length();
						double d2 = d_length * d_length;
						l.normalize();
						Vector3 L_e = light->getColor();
						// Generate shadow ray fron hit point to the light source
						Ray shadow_ray(intersection, l);
						Vector3 l_rgb = L(shadow_ray, 1, STEP_NUM, d_length, SHADOW_RAY, surfaces, materials, lights, root);
						if (l_rgb.length() != 0.) {
							double minus_l_dot_n = (-1.0 * l).dotproduct(light_area->getNormal());
							rgb += m->computeShading(l, e, N, L_e) * (1.0 / d2) * (1.0 / (SHADOWSAMPLES * SHADOWSAMPLES)) * minus_l_dot_n;
						}
					}
				}
				continue;
			}

			Vector3 l = light->getPosition() - intersection;
			double d_length = l.length();
			double d2 = d_length * d_length;
			l.normalize();
			// Generate shadow ray fron hit point to the light source
			Ray shadow_ray(intersection, l);
			Vector3 l_rgb = L(shadow_ray, 1, STEP_NUM, d_length, SHADOW_RAY, surfaces, materials, lights, root);
			if (l_rgb.length() != 0.) {
				rgb += m->computeShading(l, e, N, L_e) * (1.0 / d2);
			}
		}

		// Reflective case
		if (!(m->reflective())) {
			return rgb;
		} else {
			double d_dot_N = r._dir.dotproduct(N);
			Vector3 refl_dir = r._dir - (2 * d_dot_N) * N;
			refl_dir.normalize();
			Ray refl_r(intersection, refl_dir);
			Vector3 refl_rgb = L(refl_r, recursive_limit - 1, STEP_NUM, DOUBLE_MAX, REFLECT_RAY, surfaces, materials, lights, root);
			return rgb + m->refl().pieceMultiply(refl_rgb);
		}
	} else {
		// If there is no intersection, set the current pixel to black
		return Vector3(0, 0, 0);
	}
}

void Camera::writeRgba(const char filename[], Rgba *pixels) {
	RgbaOutputFile file(filename, _pw, _ph, WRITE_RGBA);
	file.setFrameBuffer(pixels, 1, _pw);
	file.writePixels(_ph);
}

int main(int argc, char **argv) {
	if(argc != 5) {
		cerr << "usage: raytra scenefilename outputfilename camsamples shadowsamples" << endl;
		return -1;
	}

	// Set the sample numbers
	CAMSAMPLES = atoi(argv[3]);
	SHADOWSAMPLES = atoi(argv[4]);
	// Now use the normal (BVH) mode by default
	mode = NORMAL_MODE;

	Camera cam;
	std::vector<Light *> lights;
	std::vector<Surface *> surfaces;
	std::vector<Material *> materials;

	// If no material is specified before an object, we use the default material: all black
	Material *defaultMaterial = new Material();
	materials.push_back(defaultMaterial);

	Parser parser;
	vector<Vector3 *> normals;
	parser.parse(argv[1], surfaces, materials, lights, normals, cam);
	for(int i = 0; i < normals.size(); ++i) {
		assert (normals[i]->length() - 1 < STEP_NUM);
	}


	BBoxNode* root = new BBoxNode();
	root->createTree(surfaces, 0, surfaces.size() - 1, 0); // TODO: minus 1?

#if VERBOSE
	cout << "BVH tree created" << endl;
	cout << "Number of Nodes in the tree: " << root->countNodes() << endl;
#endif

	// The yellow material used to debug is pushed to the tail of the vector
	Material* yellow = new Material(Vector3(1, 1, 0), Vector3(0, 0, 0), 0.,
			Vector3(0, 0, 0));
	materials.push_back(yellow);
	YELLOW_INDEX = materials.size() - 1;

	Vector3 lightcolor(1., 1., 1.);
	if (lights.size() == 0) {
		Light *defaultLight = new PointLight(cam.getPosition(), lightcolor);
		lights.push_back(defaultLight);
		std::cout << "No lights! Using default light instead" << std::endl;
	}


#if VERBOSE
	if(mode == NORMAL_MODE)
		cout << "Rendering using BVH mode" << endl;
		cout << "Primary ray samples: " << CAMSAMPLES * CAMSAMPLES << endl;
		cout << "Shadow ray samples: " << SHADOWSAMPLES * SHADOWSAMPLES << endl;
		cout << "Number of surfaces: " << surfaces.size() << endl;
		cout << "Number of lights: " << lights.size() << endl;
	//	cout << "Number of Nodes in BVH tree: "
#endif

	cam.render(argv[2], surfaces, materials, lights, root);

	cout << "Number of Nodes in the tree: " << root->countNodes() << endl;
//	cout << "Total count: " << COUNT << endl;
//	cout << "Total count2: " << COUNT2 << endl;

	// Dealloctating the memory
	for (Material* m : materials) {
		delete m;
	}
	materials.clear();
	surfaces.clear();
	for (Light* l : lights) {
		delete l;
	}
	lights.clear();
	for(Vector3 *v : normals)
		delete v;
	normals.clear();
	delete root;
}
