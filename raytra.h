/**
 * @author: Wode Ni
 * @version: 2016/10/12
 */

#ifndef RAYTRA_H
#define RAYTRA_H
#include <vector>
#include <algorithm>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include "material.h"
#include "basemath.h"
#include "light.h"
#include "ray.h"

class Camera {

    friend std::ostream &operator<<(std::ostream &, const Camera &);

    public:
        Camera() {}
        void initCamera (Point&, Vector3&, double&, 
                double&, double&, int&, int&);
        ~Camera();
        Ray construct_ray (int, int);
        void render(const char filename[],
                std::vector< Surface *> &surfaces,
                std::vector< Material *> &materials,
                 std::vector< Light * > &lights,
				 int mode);
        void writeRgba(const char filename[], Imf::Rgba *pixels);
        Point getPosition() const { return _eye; }
        Vector3 L (Ray& r,
        		int recursive_limit,
        		double min_t, double max_t,
        		int ray_type,
        		std::vector< Surface *> &surfaces,
                std::vector< Material *> &materials,
				std::vector< Light *> &lights);
    private:
        Point _eye;

        double _d; // Focal length

        Vector3 _u;
        Vector3 _v;
        Vector3 _w;

        double _iw, _ih;
        int _pw, _ph;
};

#endif /* RAYTRA_H */
