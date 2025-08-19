#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "EQuaternion.h"

class Cube
{
public:
	Cube(glm::vec3 frontFace,
		glm::vec3 rightFace,
		glm::vec3 leftFace,
		glm::vec3 topFace,
		glm::vec3 bottomFace,
		glm::vec3 backFace
	);

	void bindFaceColors(const unsigned int shaderID);

	void rotatePitch(float degreesPitch);
	void rotateYaw(float degreesYaw);
	void rotateRoll(float degreesRoll);

	void setStartPosition(glm::vec3 position);
	void setCurrentPosition(glm::vec3 position);
	glm::vec3 getStartingPosition();
	glm::vec3 getCurrentPosition();
	glm::vec3 getLocalRotation();

private:
	glm::vec3 faceColors[6];
	glm::vec3 startingPosition;
	glm::vec3 currentPosition;
	glm::vec3 localRotation;
};

#endif