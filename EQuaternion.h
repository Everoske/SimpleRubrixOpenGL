#ifndef QUATERNION_H
#define QUATERNION_H

#include <glm/glm.hpp>

// Simplified Quaternion class with some helper functions for rotating points about arbitrary axes
class EQuaternion
{
public:
	EQuaternion(float s, float v1, float v2, float v3) : a{ s }, v{ v1, v2, v3 } {}
	EQuaternion(float s, glm::vec3 vec) : a{ s }, v{ vec.x, vec.y, vec.z} {}
	EQuaternion(float s) : a{ s }, v{ 0.0f, 0.0f, 0.0f } {}
	EQuaternion(float v1, float v2, float v3) : a{ 0.0f }, v{ v1, v2, v3 } {}
	EQuaternion(glm::vec3 vec) : a{ 0.0f }, v{ vec.x, vec.y, vec.z } {}
	EQuaternion() : a{ 0.0f }, v{ 0.0f, 0.0f, 0.0f } {}

	float a;
	glm::vec3 v;

	float squareLength() const { return a * a + v.x * v.x + v.y * v.y + v.z * v.z; }
	float length() const { return sqrt(squareLength()); }
	EQuaternion conjugate() const { return EQuaternion(a, -v.x, -v.y, -v.z); }

	static EQuaternion getRotationQuaternion(const glm::vec3& axis, float radians);
	static glm::vec3 rotate(const glm::vec3& axis, const glm::vec3& point, float radians);
	static glm::vec3 rotatePitch(const glm::vec3& point, float radians);
	static glm::vec3 rotateYaw(const glm::vec3& point, float radians);
	static glm::vec3 rotateRoll(const glm::vec3& point, float radians);
};

EQuaternion operator*(const EQuaternion& q, const EQuaternion& p)
{
	return EQuaternion(
		q.a * p.a - glm::dot(q.v, p.v),
		q.a * p.v + p.a * q.v + glm::cross(q.v, p.v)
	);
}

EQuaternion operator*(const EQuaternion& q, const glm::vec3& v)
{
	return q * EQuaternion(v);
}

EQuaternion EQuaternion::getRotationQuaternion(const glm::vec3& axis, float radians)
{
	glm::vec3 n = glm::normalize(axis);
	float radiansD2 = radians / 2;
	return EQuaternion(glm::cos(radiansD2), glm::sin(radiansD2) * n.x, glm::sin(radiansD2) * n.y, glm::sin(radiansD2) * n.z);
}

glm::vec3 EQuaternion::rotate(const glm::vec3& axis, const glm::vec3& point, float radians)
{
	EQuaternion q = getRotationQuaternion(axis, radians);
	EQuaternion qC = q.conjugate();
	EQuaternion qU = q * point;
	EQuaternion qUqC = qU * qC;
	return qUqC.v;
}

glm::vec3 EQuaternion::rotatePitch(const glm::vec3& point, float radians)
{
	return rotate(glm::vec3(1.0f, 0.0f, 0.0f), point, radians);
}

glm::vec3 EQuaternion::rotateYaw(const glm::vec3& point, float radians)
{
	return rotate(glm::vec3(0.0f, 1.0f, 0.0f), point, radians);
}

glm::vec3 EQuaternion::rotateRoll(const glm::vec3& point, float radians)
{
	return rotate(glm::vec3(0.0f, 0.0f, 1.0f), point, radians);
}

#endif