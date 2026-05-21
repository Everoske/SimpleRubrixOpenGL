#include "AABBCollider.h"


bool AABBCollider::checkPointCollision(glm::vec3 point) const
{
	float rightXBounds = mPosition.x + mHalfExtents.x;
	float leftXBounds = mPosition.x - mHalfExtents.x;
	float upperYBounds = mPosition.y + mHalfExtents.y;
	float lowerYBounds = mPosition.y - mHalfExtents.y;
	float upperZBounds = mPosition.z + mHalfExtents.z;
	float lowerZBounds = mPosition.z - mHalfExtents.z;

	if (point.x > rightXBounds || point.x < leftXBounds)
		return false;
	if (point.y > upperYBounds || point.y < lowerYBounds)
		return false;
	if (point.z > upperZBounds || point.z < lowerZBounds)
		return false;

	return true;
}