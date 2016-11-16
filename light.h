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
		_udirection = udirection;
		_direction = direction;
	}

	virtual Vector3 getColor() const override { return _color; }

	virtual bool isAmbient() override { return false; }

private:
	Vector3 _color;
	double _len;
	Vector3 _direction;
	Vector3 _udirection;
};



#endif /* LIGHT_H_ */
