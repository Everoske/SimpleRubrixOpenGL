#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

struct Ray
{
	// Ray origin point
	glm::vec3 Origin;

	// Direction
	glm::vec3 Direction;

	float Length;

	Ray(glm::vec3 origin,
		glm::vec3 direction,
		float length) :
		Origin{ origin },
		Direction{ direction },
		Length{ length }
	{
	}
};


#endif