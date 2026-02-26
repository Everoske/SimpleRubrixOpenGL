#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Quaternion.h"

class Cube
{
public:
	Cube(glm::vec3 frontFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 rightFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 leftFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 topFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 bottomFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 backFace = glm::vec3(0.05f, 0.05f, 0.05f)
	);

	void bindFaceColors(const unsigned int shaderID);

	void rotateSmoothX(float radians, float timePercent);
	void rotateSmoothY(float radians, float timePercent);
	void rotateSmoothZ(float radians, float timePercent);

	void setStartPosition(const glm::vec3& position);
	void setCurrentPosition(const glm::vec3& position);
	void setOrientation(const Quaternion& newOrientation);
	glm::vec3 getStartingPosition() const;
	glm::vec3 getCurrentPosition() const;
	Quaternion getOrientation() const;
	glm::mat4x4 getTransformationMatrix() const;

private:
	glm::vec3 faceColors[6];
	glm::vec3 startingPosition;
	glm::vec3 currentPosition;
	glm::vec3 lastFixedPosition;
	Quaternion orientation;
	Quaternion lastFixedOrientation;
	glm::vec3 scale;
	glm::vec3 up;
	glm::vec3 forward;

	bool isRotating;

	void rotateVectors(glm::vec3 axis, float radians);
	void rotateVectors(Quaternion newOrientation);
};

#endif