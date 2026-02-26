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
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, radians);
	Quaternion targetOrientation = targetRotator * lastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(1.0f, 0.0f, 0.0f), lastFixedPosition, radians);
		setCurrentPosition(newPosition);
		std::cout << currentPosition.x << "," << currentPosition.y << "," << currentPosition.z << std::endl;
		isRotating = false;
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
		std::cout << currentPosition.x << "," << currentPosition.y << "," << currentPosition.z << std::endl;
		isRotating = false;
	}

	orientation = Quaternion::slerp(lastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(lastFixedPosition, glm::vec3(0.0f, 0.0f, 1.0f), radians, timePercent);
	currentPosition = newPosition;
}

void Cube::setStartPosition(const glm::vec3& position)
{
	startingPosition = position;
	setCurrentPosition(position);
}

void Cube::setCurrentPosition(const glm::vec3& position)
{
	currentPosition = position;

	// Reset orientation to Quaternion identity if cube returns to original position
	if (currentPosition == startingPosition)
	{
		orientation = Quaternion();
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		forward = glm::vec3(0.0f, 0.0f, -1.0f);
	}
}

void Cube::setOrientation(const Quaternion& newOrientation)
{
	orientation = newOrientation;
	rotateVectors(orientation);
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

void Cube::rotateVectors(glm::vec3 axis, float radians)
{
	up = glm::normalize(Quaternion::rotatePoint(axis, up, radians));
	forward = glm::normalize(Quaternion::rotatePoint(axis, forward, radians));
}

void Cube::rotateVectors(Quaternion newOrientation)
{
	up = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 1.0f, 0.0f)));
	forward = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 0.0f, -1.0f)));
}
