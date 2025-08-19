#include "EQuaternion.h"

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