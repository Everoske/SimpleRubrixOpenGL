#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include <glm/vec3.hpp>

class AABBCollider
{
public:
	AABBCollider(glm::vec3 halfExtents, glm::vec3 position) : mHalfExtents{ halfExtents }, mPosition{ position } {}

	bool checkPointCollision(glm::vec3 point) const;

	glm::vec3 getHalfExtents() const { return mHalfExtents; }
	glm::vec3 getPosition() const { return mPosition; }
	glm::vec3 getMinBounds() const { return mPosition - mHalfExtents; }
	glm::vec3 getMaxBounds() const { return mPosition + mHalfExtents; }

	void setPosition(glm::vec3 newPosition) { mPosition = newPosition; }

private:
	glm::vec3 mHalfExtents;
	glm::vec3 mPosition;
};

#endif