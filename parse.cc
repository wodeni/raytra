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

void Parser::parse(const char *file, std::vector<Surface *> &surfaces,
		std::vector<Material *> &materials, std::vector<Light *> &lights,
		Camera &cam) {
	ifstream in(file);
	char buffer[1025];
	string cmd;

	int currentMaterial = 0; // 0 is the default material
	int cameracount = 0;
	int ambientcount = 0;

	for (int line = 1; in.good(); line++) {
		in.getline(buffer, 1024);
		buffer[in.gcount()] = 0;

		cmd = "";

		istringstream iss(buffer);

		Surface *cursurface = 0;

		iss >> cmd;
		if (cmd[0] == '/' or cmd.empty()) {
			continue;
		} else if (cmd == "s") {
			// Sphere
			Point pos;
			double r;
			iss >> pos >> r;
			cursurface = new Sphere(pos, r);
#if DEBUG
			cout << "got a sphere with ";
			cout << "Position: " << pos << " Radius: " << r << endl;
#endif
		} else if (cmd == "t") {
			// triangle
			Point a, b, c;
			iss >> a >> b >> c;
			cursurface = new Triangle(a, b, c);
		} else if (cmd == "p") {
			// Plane
			Vector3 n;
			double d;
			iss >> n >> d;
			cursurface = new Plane(n, d);
		} else if (cmd == "c") {
			// Camera
			cameracount++;
			Point pos;
			Vector3 dir;
			double d, iw, ih;
			int pw, ph;
			iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
			cam.initCamera(pos, dir, d, iw, ih, pw, ph);
#if DEBUG
			cout << "got a camera with ";
			cout << "Position: " << pos << " Direction: " << dir << " Focal length: " << d << " Film size: " << iw << "x" << ih << endl;
			cout << "Output size: " << pw << "x" << ph << endl;
#endif
		} else if (cmd == "l") {
			// Light
			iss >> cmd;
			if (cmd == "p") {
				// Point Light
				Point pos;
				Vector3 rgb;
				iss >> pos >> rgb;
				lights.push_back(new PointLight(pos, rgb));
			} else if (cmd == "d") {
				Vector3 dir, rgb;
				iss >> dir >> rgb;
			} else if (cmd == "a") {
				// Ambient Light
				Vector3 rgb;
				iss >> rgb;
				lights.push_back(new AmbientLight(rgb));
				ambientcount++;
			} else {
				cout << "Parser error: invalid light at line " << line << endl;
			}
		} else if (cmd == "m") {
			Vector3 diff, spec, refl;
			double r;
			iss >> diff >> spec >> r >> refl;
			Material *thismaterial = new Material(diff, spec, r, refl);
			materials.push_back(thismaterial);
			++currentMaterial;
		} else if (cmd == "w") {
			string filename;
			iss >> filename;
			vector <Surface *> triangles;
			read_wavefront_file(filename.c_str(), triangles);
			for(Surface *t : triangles) {
				t->setmaterialid(currentMaterial);
				surfaces.push_back(t);
			}

		} else {
			cout << "Parser error: invalid command at line " << line << endl;
		}

		if (cursurface) {
			cursurface->setmaterialid(currentMaterial);
			surfaces.push_back(cursurface);
		}
	}
	in.close();
	// Make sure we have some surfaces to render
	assert(surfaces.size() != 0);
	// As mentioned in class #5, we allow only one camera in the scene
	assert(cameracount == 1);
	// As specified in hw 1.3, only one ambient light is permitted
	assert(ambientcount <= 1);
#if DEBUG 
	cout << "Summary: " << surfaces.size() << " surfaces scanned " << materials.size() << " materials scanned" << endl;
#endif
}

// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices:
//
//     verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2],
//     verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2],
//     verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]
//
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//

void Parser::read_wavefront_file(const char *file, std::vector<Surface *> &triangles) {

	vector<int> tris;
	vector<float> verts;

	ifstream in(file);
	char buffer[1025];
	string cmd;

	for (int line = 1; in.good(); line++) {
		in.getline(buffer, 1024);
		buffer[in.gcount()] = 0;

		cmd = "";

		istringstream iss(buffer);

		iss >> cmd;

		if (cmd[0] == '#' or cmd.empty()) {
			// ignore comments or blank lines
			continue;
		} else if (cmd == "v") {
			// got a vertex:

			// read in the parameters:
			double pa, pb, pc;
			iss >> pa >> pb >> pc;

			verts.push_back(pa);
			verts.push_back(pb);
			verts.push_back(pc);
		} else if (cmd == "f") {
			// got a face (triangle)

			// read in the parameters:
			int i, j, k;
			iss >> i >> j >> k;

			// vertex numbers in OBJ files start with 1, but in C++ array
			// indices start with 0, so we're shifting everything down by
			// 1
			tris.push_back(i - 1);
			tris.push_back(j - 1);
			tris.push_back(k - 1);
		} else {
			std::cerr << "Parser error: invalid command at line " << line
					<< std::endl;
		}

	}

	in.close();

	//   std::cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << std::endl;

	for (int i = 0; i < tris.size() / 3.0; ++i) {
		Point a (verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2]);
		Point b (verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2]);
		Point c (verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]);
		triangles.push_back(new Triangle(a, b, c));
	}
}
