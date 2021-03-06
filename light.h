/*
 * light.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Wode "Nimo" Ni
 */

#ifndef LIGHT_H_
#define LIGHT_H_


class Light {
public:
	Light()
		: _position( Point(0, 0, 0) ) {}

	virtual ~Light() {}

	virtual Vector3 getColor() const = 0;

	virtual bool isAmbient() = 0;

	virtual bool isArea() = 0;

	Point getPosition() const { return _position; }

protected:

	Point _position;

};

class PointLight : public Light {
public:
	PointLight () {}

	PointLight (const Point &position, const Vector3 &color) {
		_position = position;
		_color = color;
	}

	virtual Vector3 getColor() const override { return _color; }

	virtual bool isAmbient() override { return false; }

	virtual bool isArea() override { return false; }


private:

	Vector3 _color;

};

class AmbientLight : public Light {
public:

	AmbientLight () {}

	AmbientLight (const Vector3 &color) {
		_color = color;
	}

	virtual Vector3 getColor() const override { return _color; }

	virtual bool isAmbient() override { return true; }

	virtual bool isArea() override { return false; }


private:

	Vector3 _color;

};

class AreaLight : public Light {
public:
	AreaLight(const Point position,
			const Vector3 direction,
			const Vector3 udirection,
			double len, const
			Vector3 color) {
		_position = position;
		_color = color;
		_len = len;
		_u = udirection;
		_direction = direction;
		_v = _u.crossproduct(_direction);
		_v.normalize();
		// 	Precompute the coordinate of the lower left corner
		Vector3 offset = (-1.0 *_len / 2) * _u + (-1.0 * _len / 2) * _v;
		_lowerleft = offset + position;
	}

	virtual Vector3 getColor() const override { return _color; }

	virtual bool isAmbient() override { return false; }

	virtual bool isArea() override { return true; }

	Vector3 getNormal() const { return _direction; }

	Point createSample(double ru, double rv) {
		Point newPos = ru * _u * _len + rv * _v * _len + _lowerleft;
#if VERBOSE
//		cout << "Generated a sample on area light: " << newPos << ", Center of the light is: " << _position << endl;
#endif
		return newPos;
	}


private:
	Vector3 _color;
	double _len;
	Vector3 _direction;
	Vector3 _u, _v;
	Point _lowerleft;
};



#endif /* LIGHT_H_ */
