#pragma once

#include <cmath>

class Vector3
{
public:
	Vector3()
		:x(0.0f), y(0.0f), z(0.0f) {}

	Vector3(float x, float y, float z)
		:x(x), y(y), z(z) {}

	float Magnitude();
	float InnerProduct(Vector3 v);

	Vector3 operator-(Vector3 v);

	Vector3 operator+(Vector3 v);
	Vector3& operator+=(Vector3 v);

	Vector3 operator/(float val);
	Vector3& operator/=(float val);

	Vector3 CrossProduct(Vector3 v);
	void Normalize();

	float x;
	float y;
	float z;

};