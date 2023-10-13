#pragma once
#include <string>

class Vector2 {
private:
	float _x;
	float _y;
public:
	inline Vector2() {
		_x = 0;
		_y = 0;
	};

	inline Vector2(float x, float y) {
		_x = x;
		_y = y;
	};

	inline float x() {
		return _x;
	};

	inline float y() {
		return _y;
	};

	inline Vector2 operator +=(Vector2 right) {
		this->_x += right.x();
		this->_y += right.y();
		return *this;
	};

	inline Vector2 operator -=(Vector2 right) {
		this->_x -= right.x();
		this->_y -= right.y();
		return *this;
	};

	inline Vector2 operator *=(float right) {
		this->_x *= right;
		this->_y *= right;
		return *this;
	};

	static inline float SqrDistance(Vector2 left, Vector2 right) {
		float o_x = left.x() - right.x();
		float o_y = left.y() - right.y();
		return (o_x * o_x) + (o_y * o_y);
	};

	inline float SqrMagnitude() {
		return (x() * x()) + (y() * y());
	};

	inline float Magnitude() {
		return sqrt(SqrMagnitude());
	};

	inline Vector2 Normalize() {
		if (SqrMagnitude() > 0) {
			*this *= (1 / Magnitude());
			return *this;
		}
		else {
			return *this;
		}
	};

	inline std::string ToString(int digits) {
		return std::to_string(x()).substr(0, digits) + ":" + std::to_string(y()).substr(0, digits);
	};

};
