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
	orientation(Quaternion())
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
	currentPosition = Quaternion::rotatePitch(currentPosition, glm::radians(degreesPitch));
	orientation = Quaternion::rotateQuaternion(orientation, glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-degreesPitch));
}

void Cube::rotateYaw(float degreesYaw)
{
	currentPosition = Quaternion::rotateYaw(currentPosition, glm::radians(degreesYaw));
	orientation = Quaternion::rotateQuaternion(orientation, glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-degreesYaw));
}

void Cube::rotateRoll(float degreesRoll)
{
	currentPosition = Quaternion::rotateRoll(currentPosition, glm::radians(degreesRoll));
	orientation = Quaternion::rotateQuaternion(orientation, glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(-degreesRoll));
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

Quaternion Cube::getOrientation()
{
	return orientation;
}
