#include "CollisionSystem.h"
#include <algorithm>

CollisionSystem* CollisionSystem::Instance()
{
	static CollisionSystem instance;
	return &instance;
}

void CollisionSystem::generateAABBCollider(int cubeID, glm::vec3 halfExtents, glm::vec3 cubePosition)
{
	mCollisionMap.insert(std::make_pair(cubeID, AABBCollider(halfExtents, cubePosition)));
}

void CollisionSystem::removeAABBCollider(int cubeID)
{
	mCollisionMap.erase(mCollisionMap.find(cubeID));
}

void CollisionSystem::updateColliderPosition(int cubeID, glm::vec3 newPosition)
{
	CollisionMap::iterator ent = mCollisionMap.find(cubeID);
	if (ent != mCollisionMap.end())
	{
		ent->second.setPosition(newPosition);
	}
}

bool CollisionSystem::isCloser(glm::vec3 newPoint, glm::vec3 oldPoint, glm::vec3 origin) const
{
	glm::vec3 v1 = oldPoint - origin;
	glm::vec3 v2 = newPoint - origin;
	float sqrMag1 = v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;
	float sqrMag2 = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;

	return sqrMag2 < sqrMag1;
}

bool CollisionSystem::intersectRayAllAABB(glm::vec3 origin, glm::vec3 end, int& closestCubeID) const
{
	glm::vec3 closestPoint = end;
	float fMin;
	glm::vec3 lastClosest;

	CollisionMap::const_iterator it;
	bool rayHit = false;

	for (it = mCollisionMap.begin(); it != mCollisionMap.end(); it++)
	{
		if (intersectRayAABB(origin, end, it->second, fMin, lastClosest))
		{
			rayHit = true;
			if (isCloser(lastClosest, closestPoint, origin))
			{
				closestCubeID = it->first;
				closestPoint = lastClosest;
			}
		}
	}

	return rayHit;
}

bool CollisionSystem::intersectRayAllAABB(glm::vec3 origin, glm::vec3 end, int& closestCubeID, glm::vec3& collisionPoint) const
{
	glm::vec3 closestPoint = end;
	float fMin;
	glm::vec3 lastClosest;

	CollisionMap::const_iterator it;
	bool rayHit = false;

	for (it = mCollisionMap.begin(); it != mCollisionMap.end(); it++)
	{
		if (intersectRayAABB(origin, end, it->second, fMin, lastClosest))
		{
			rayHit = true;
			if (isCloser(lastClosest, closestPoint, origin))
			{
				closestCubeID = it->first;
				closestPoint = lastClosest;
			}
		}
	}

	collisionPoint = closestPoint;

	return rayHit;
}

bool CollisionSystem::intersectRayAABB(glm::vec3 origin, glm::vec3 end, const AABBCollider& cube,
	float& fMin, glm::vec3& collisionPoint) const
{
	fMin = 0.0f;
	float fMax = 1.0f;

	glm::vec3 minBounds = cube.getMinBounds();
	glm::vec3 maxBounds = cube.getMaxBounds();
	glm::vec3 ray = end - origin;

	for (int i = 0; i < 3; i++)
	{
		if (abs(ray[i]) < FLT_EPSILON)
		{
			// Ray is parallel to the slab. No hit if origin not within slab
			if (origin[i] < minBounds[i] || origin[i] > maxBounds[i])
				return false;
		}
		else
		{
			// Calculate intersection fractions for where the ray intersects with the slab
			float f1 = (minBounds[i] - origin[i]) / ray[i];
			float f2 = (maxBounds[i] - origin[i]) / ray[i];

			// F1 should be closer to the origin. F2 should be farther away
			if (f1 > f2)
				std::swap(f1, f2);

			fMin = std::max(fMin, f1);
			fMax = std::min(fMax, f2);

			// Exit with no collision as soon as slab intersection becomes empty
			if (fMin > fMax)
				return false;
		}
	}

	// Calculate intersection
	collisionPoint = origin + ray * fMin;

	return true;
}
