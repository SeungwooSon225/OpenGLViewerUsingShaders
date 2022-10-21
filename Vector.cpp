#include "VECTOR.h"


float Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::InnerProduct(Vector3 v)
{
	return (x * v.x + y * v.y + z * v.z);
}

Vector3 Vector3::operator+(Vector3 v)
{
	Vector3 result = (*this);
	result.x += v.x;
	result.y += v.y;
	result.z += v.z;

	return result;
}

Vector3& Vector3::operator+=(Vector3 v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3 Vector3::operator/(float val)
{
	Vector3 result = (*this);
	result.x /= val;
	result.y /= val;
	result.z /= val;

	return result;
}

Vector3& Vector3::operator/=(float val)
{
	this->x /= val;
	this->y /= val;
	this->z /= val;

	return *this;
}

Vector3 Vector3::operator-(Vector3 v)
{
	Vector3 result = (*this);
	result.x -= v.x;
	result.y -= v.y;
	result.z -= v.z;

	return result;
}

Vector3 Vector3::CrossProduct(Vector3 v)
{
	Vector3 result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;

	return result;
}


void Vector3::Normalize()
{
	float w = Magnitude();
	x /= w;
	y /= w;
	z /= w;
}