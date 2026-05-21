#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <map>
#include <glm/vec3.hpp>
#include "AABBCollider.h"

class CollisionSystem
{
private:
	typedef std::map<int, AABBCollider> CollisionMap;

private:
	CollisionMap mCollisionMap;

	CollisionSystem() {}

public:
	CollisionSystem(const CollisionSystem& obj) = delete;

	static CollisionSystem* Instance();

	void generateAABBCollider(int cubeID, glm::vec3 halfExtents, glm::vec3 cubePosition);
	void removeAABBCollider(int cubeID);
	void updateColliderPosition(int cubeID, glm::vec3 newPosition);
	bool isCloser(glm::vec3 newPoint, glm::vec3 oldPoint, glm::vec3 origin) const;
	bool intersectRayAllAABB(glm::vec3 origin, glm::vec3 end, int& closestCubeID) const;
	bool intersectRayAllAABB(glm::vec3 origin, glm::vec3 end, int& closestCubeID, glm::vec3& collisionPoint) const;
	bool intersectRayAABB(glm::vec3 origin, glm::vec3 end, const AABBCollider& cube, float& fMin, glm::vec3& collisionPoint) const;
};

#endif