#include "Quaternion.h"

float Quaternion::squareLength() const
{
	return w * w + v.x * v.x + v.y * v.y + v.z * v.z;
}
float Quaternion::length() const
{
	return glm::sqrt(squareLength());
}

float Quaternion::dot(const Quaternion& q) const
{
	return w * q.w + glm::dot(v, q.v);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, -v.x, -v.y, -v.z);
}

glm::mat4 Quaternion::toRotationMatrix() const
{
	glm::mat4x4 rotationMatrix =
	{
		1 - 2 * v.y * v.y - 2 * v.z * v.z, 2 * v.x * v.y - 2 * w * v.z, 2 * v.x * v.z + 2 * w * v.y, 0,
		2 * v.x * v.y + 2 * w * v.z, 1 - 2 * v.x * v.x - 2 * v.z * v.z, 2 * v.y * v.z - 2 * w * v.x, 0,
		2 * v.x * v.z - 2 * w * v.y, 2 * v.y * v.z + 2 * w * v.x, 1 - 2 * v.x * v.x - 2 * v.y * v.y, 0,
		0, 0, 0, 1
	};
	return rotationMatrix;
}

Quaternion Quaternion::getRotationQuaternion(const glm::vec3& axis, float radians)
{
	glm::vec3 n = glm::normalize(axis);
	float radiansD2 = radians / 2;
	return Quaternion(glm::cos(radiansD2), glm::sin(radiansD2) * n.x, glm::sin(radiansD2) * n.y, glm::sin(radiansD2) * n.z);
}

Quaternion Quaternion::rotateQuaternion(const Quaternion orientation, const glm::vec3& axis, float radians)
{
	Quaternion rotator = getRotationQuaternion(axis, radians);
	return rotator * orientation;
}

glm::vec3 Quaternion::rotatePoint(const glm::vec3& axis, const glm::vec3& point, float radians)
{
	Quaternion q = getRotationQuaternion(axis, radians);
	Quaternion qC = q.conjugate();
	Quaternion qU = q * point;
	Quaternion qUqC = qU * qC;
	return qUqC.v;
}

glm::vec3 Quaternion::rotatePoint(const Quaternion& rotator, const glm::vec3& point)
{
	Quaternion q = (rotator * point) * rotator.conjugate();
	return q.v;
}

Quaternion Quaternion::slerp(Quaternion q1, Quaternion q2, float t)
{
	float cosOmega = q1.dot(q2);
	if (cosOmega < 0.0f)
	{
		q2 = Quaternion(-q2.w, -q2.v);
		cosOmega = -cosOmega;
	}

	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = glm::sqrt(1.0f - cosOmega * cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = glm::sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = glm::sin(t * omega) * oneOverSinOmega;
	}

	float w = q1.w * k0 + q2.w * k1;
	float x = q1.v.x * k0 + q2.v.x * k1;
	float y = q1.v.y * k0 + q2.v.y * k1;
	float z = q1.v.z * k0 + q2.v.z * k1;

	return Quaternion(w, x, y, z);
}

glm::vec3 Quaternion::slerpPoint(const glm::vec3& point, const glm::vec3& axis, float radians, float t)
{
	Quaternion finalRotator = Quaternion::getRotationQuaternion(axis, radians);
	Quaternion rotator = Quaternion::slerp(Quaternion(), finalRotator, t);
	Quaternion rotatedPoint = (rotator * point) * rotator.conjugate();
	return rotatedPoint.v;
}
