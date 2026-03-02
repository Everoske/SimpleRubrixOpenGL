#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Cube::Cube(glm::vec3 frontFace,
	glm::vec3 rightFace,
	glm::vec3 leftFace,
	glm::vec3 topFace,
	glm::vec3 bottomFace,
	glm::vec3 backFace
) :
	startingPosition(glm::vec3(0.0f)),
	currentPosition(glm::vec3(0.0f)),
	orientation(Quaternion()),
	scale(0.5f, 0.5f, 0.5f),
	up(glm::vec3(0.0f, 1.0f, 0.0f)),
	forward(glm::vec3(0.0f, 0.0f, -1.0f))
{
	faceColors[0] = frontFace;
	faceColors[1] = rightFace;
	faceColors[2] = leftFace;
	faceColors[3] = topFace;
	faceColors[4] = bottomFace;
	faceColors[5] = backFace;
	highlight = false;

	isRotating = false;
	lastFixedOrientation = orientation;
	lastFixedPosition = currentPosition;
}

void Cube::bindFaceColors(const unsigned int shaderID)
{
	glUniform3fv(glGetUniformLocation(shaderID, "frontColor"), 1, &faceColors[0][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "rightColor"), 1, &faceColors[1][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "leftColor"), 1, &faceColors[2][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "topColor"), 1, &faceColors[3][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "bottomColor"), 1, &faceColors[4][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "backColor"), 1, &faceColors[5][0]);
	glUniform1i(glGetUniformLocation(shaderID, "highlight"), highlight);
}

void Cube::rotateSmoothX(float radians, float timePercent)
{
	if (!isRotating)
	{
		isRotating = true;
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	glm::vec3 rightVector = glm::cross(up, forward);
	rightVector = glm::vec3(clampNormalScalar(rightVector.x), clampNormalScalar(rightVector.y), clampNormalScalar(rightVector.z));
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(1.0f, 0.0f, 0.0f), lastFixedPosition, radians);
		setCurrentPosition(newPosition);
		isRotating = false;
		return;
	}
	
	orientation = Quaternion::slerp(lastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(lastFixedPosition, glm::vec3(1.0f, 0.0f, 0.0f), radians, timePercent);
	currentPosition = newPosition;
}

void Cube::rotateSmoothY(float radians, float timePercent)
{
	if (!isRotating)
	{
		isRotating = true;
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(up, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, -1.0f, 0.0f), lastFixedPosition, radians);
		setCurrentPosition(newPosition);
		isRotating = false;
		return;
	}

	orientation = Quaternion::slerp(lastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(lastFixedPosition, glm::vec3(0.0f, -1.0f, 0.0f), radians, timePercent);
	currentPosition = newPosition;
}

void Cube::rotateSmoothZ(float radians, float timePercent)
{
	if (!isRotating)
	{
		isRotating = true;
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(forward, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, 0.0f, 1.0f), lastFixedPosition, radians);
		setCurrentPosition(newPosition);
		isRotating = false;
		return;
	}

	orientation = Quaternion::slerp(lastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(lastFixedPosition, glm::vec3(0.0f, 0.0f, 1.0f), radians, timePercent);
	currentPosition = newPosition;
}

void Cube::rotateXImmediate(float radians)
{
	if (isRotating)
	{
		isRotating = false;
	}
	else
	{
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	glm::vec3 rightVector = glm::cross(up, forward);
	rightVector = glm::vec3(clampNormalScalar(rightVector.x), clampNormalScalar(rightVector.y), clampNormalScalar(rightVector.z));
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(1.0f, 0.0f, 0.0f), lastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::rotateYImmediate(float radians)
{
	if (isRotating)
	{
		isRotating = false;
	}
	else
	{
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(up, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, -1.0f, 0.0f), lastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::rotateZImmediate(float radians)
{
	if (isRotating)
	{
		isRotating = false;
	}
	else
	{
		lastFixedPosition = currentPosition;
		lastFixedOrientation = orientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(forward, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, 0.0f, 1.0f), lastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::setStartPosition(const glm::vec3& position)
{
	startingPosition = position;
	setCurrentPosition(position);
}

void Cube::setCurrentPosition(const glm::vec3& position)
{
	currentPosition = position;
}

void Cube::setOrientation(const Quaternion& newOrientation)
{
	orientation = newOrientation;
	//std::cout << orientation.v.x << ", " << orientation.v.y << ", " << orientation.v.z << std::endl;
	rotateVectors(orientation);
}

void Cube::setHighlight(bool highlightCube)
{
	highlight = highlightCube;
}

glm::vec3 Cube::getStartingPosition() const
{
	return startingPosition;
}

glm::vec3 Cube::getCurrentPosition() const
{
	return currentPosition;
}

Quaternion Cube::getOrientation() const
{
	return orientation;
}

glm::mat4x4 Cube::getTransformationMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, currentPosition) * orientation.toRotationMatrix();
	model = glm::scale(model, scale);
	return model;
}

bool Cube::isInSolvedPositionAndOrientation()
{
	return currentPosition == startingPosition && up.y == 1.0f && forward.z == -1.0f;
}

// Try to clamp vectors???
void Cube::rotateVectors(glm::vec3 axis, float radians)
{
	up = glm::normalize(Quaternion::rotatePoint(axis, up, radians));
	forward = glm::normalize(Quaternion::rotatePoint(axis, forward, radians));
}

// Try to clamp vectors??
void Cube::rotateVectors(Quaternion newOrientation)
{
	up = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 1.0f, 0.0f)));
	forward = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 0.0f, -1.0f)));
	up = glm::vec3(clampNormalScalar(up.x), clampNormalScalar(up.y), clampNormalScalar(up.z));
	forward = glm::vec3(clampNormalScalar(forward.x), clampNormalScalar(forward.y), clampNormalScalar(forward.z));
	recalculateOrientation();
}

float Cube::clampNormalScalar(float scalar)
{
	if (scalar > 0.5f)
		return 1.0f;
	if (scalar < -0.5f)
		return -1.0f;
	
	return 0.0f;
}

void Cube::recalculateOrientation()
{
	if (up.y == 1.0f && forward.z == -1.0f)
	{
		//std::cout << "In Original Position" << std::endl;
		orientation = Quaternion();
	}
	else if (up.y == 1.0f)
	{
		//std::cout << "Up in original Position" << std::endl;
		float yAngle = 0.0f;
		if (forward.z == 1.0f)
			yAngle = glm::radians(180.0f);
		else if (forward.x == 1.0f)
			yAngle = glm::radians(-90.0f);
		else if (forward.x == -1.0f)
			yAngle = glm::radians(90.0f);
		Quaternion rotator = Quaternion::getRotationQuaternion(up, yAngle);
		orientation = rotator * Quaternion();

	}
	else if (forward.z == -1.0f)
	{
		//std::cout << "Forward in original Position" << std::endl;
		float zAngle = 0.0f;
		if (up.y == -1.0f)
			zAngle = glm::radians(180.0f);
		else if (up.x == 1.0f)
			zAngle = glm::radians(90.0f);
		else if (up.x == -1.0f)
			zAngle = glm::radians(-90.0f);
		Quaternion rotator = Quaternion::getRotationQuaternion(forward, zAngle);
		orientation = rotator * Quaternion();
	}
}
