#ifndef QUATERNION_H
#define QUATERNION_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

/// <summary>
/// Simple class for representing Quaternions with some helper methods for rotations.
/// </summary>
class Quaternion
{
public:
	// TODO: Try switching to doubles
	Quaternion() : w{ 1.0f }, v{ 0.0f } {}
	Quaternion(float s, float v1, float v2, float v3) : w{ s }, v{ v1, v2, v3 } {}
	Quaternion(float s, glm::vec3 vec) : w{ s }, v{ vec.x, vec.y, vec.z } {}
	Quaternion(glm::vec3 vec) : w{ 0.0f }, v{ vec.x, vec.y, vec.z } {}

	float squareLength() const;
	float length() const;
	float dot(const Quaternion& q) const;
	Quaternion conjugate() const;
	glm::mat4 toRotationMatrix() const;

	static Quaternion getRotationQuaternion(const glm::vec3& axis, float radians);
	static Quaternion rotateQuaternion(const Quaternion orientation, const glm::vec3& axis, float radians);
	static glm::vec3 rotatePoint(const glm::vec3& axis, const glm::vec3& point, float radians);
	static glm::vec3 rotatePoint(const Quaternion& rotator, const glm::vec3& point);
	static Quaternion slerp(Quaternion q1, Quaternion q2, float t);
	static glm::vec3 slerpPoint(const glm::vec3& point, const glm::vec3& axis, float radians, float t);

	float w;
	glm::vec3 v;

	friend Quaternion operator*(const Quaternion& q, const Quaternion& p)
	{
		return Quaternion(
			q.w * p.w - glm::dot(q.v, p.v),
			q.w * p.v + p.w * q.v + glm::cross(q.v, p.v)
		);
	}

	friend Quaternion operator*(const Quaternion& q, const glm::vec3& v)
	{
		return q * Quaternion(v);
	}
};


#endif