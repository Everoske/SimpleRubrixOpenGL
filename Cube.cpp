#include "Cube.h"

Cube::Cube(glm::vec3 frontFace,
	glm::vec3 rightFace,
	glm::vec3 leftFace,
	glm::vec3 topFace,
	glm::vec3 bottomFace,
	glm::vec3 backFace
) :
	startingPosition(glm::vec3(0.0f)),
	currentPosition(glm::vec3(0.0f)),
	localRotation(glm::vec3(0.0f))
{
	faceColors[0] = frontFace;
	faceColors[1] = rightFace;
	faceColors[2] = leftFace;
	faceColors[3] = topFace;
	faceColors[4] = bottomFace;
	faceColors[5] = backFace;
}

void Cube::bindFaceColors(const unsigned int shaderID)
{
	glUniform3fv(glGetUniformLocation(shaderID, "frontColor"), 1, &faceColors[0][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "rightColor"), 1, &faceColors[1][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "leftColor"), 1, &faceColors[2][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "topColor"), 1, &faceColors[3][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "bottomColor"), 1, &faceColors[4][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "backColor"), 1, &faceColors[5][0]);
}

void Cube::rotatePitch(float degreesPitch)
{
	currentPosition = EQuaternion::rotatePitch(currentPosition, glm::radians(degreesPitch));
	localRotation.x += glm::radians(degreesPitch);
}

void Cube::rotateYaw(float degreesYaw)
{
	currentPosition = EQuaternion::rotateYaw(currentPosition, glm::radians(degreesYaw));
	localRotation.y += glm::radians(degreesYaw);
}

void Cube::rotateRoll(float degreesRoll)
{
	currentPosition = EQuaternion::rotateRoll(currentPosition, glm::radians(degreesRoll));
	localRotation.z += glm::radians(degreesRoll);
}

void Cube::setStartPosition(glm::vec3 position)
{
	startingPosition = position;
	setCurrentPosition(position);
}

void Cube::setCurrentPosition(glm::vec3 position)
{
	currentPosition = position;
}

glm::vec3 Cube::getStartingPosition()
{
	return startingPosition;
}

glm::vec3 Cube::getCurrentPosition()
{
	return currentPosition;
}

glm::vec3 Cube::getLocalRotation()
{
	return localRotation;
}