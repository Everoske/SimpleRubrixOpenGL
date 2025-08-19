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
	float length() const { return glm::sqrt(squareLength()); }
	EQuaternion conjugate() const { return EQuaternion(a, -v.x, -v.y, -v.z); }

	static EQuaternion getRotationQuaternion(const glm::vec3& axis, float radians);
	static glm::vec3 rotate(const glm::vec3& axis, const glm::vec3& point, float radians);
	static glm::vec3 rotatePitch(const glm::vec3& point, float radians);
	static glm::vec3 rotateYaw(const glm::vec3& point, float radians);
	static glm::vec3 rotateRoll(const glm::vec3& point, float radians);
};

#endif