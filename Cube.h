#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "EQuaternion.h"

class Cube
{
public:
	Cube(glm::vec3 frontFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 rightFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 leftFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 topFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 bottomFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 backFace = glm::vec3(0.05f, 0.05f, 0.05f)
		) :
		startingPosition(glm::vec3(0.0f)),
		currentPosition(glm::vec3(0.0f)),
		localRotation(glm::vec3(0.0f)),
		initialWorldRotation(glm::vec3(0.0f)),
		worldRotation(glm::vec3(0.0f)),
		magnitude(0.0f)
	{
		faceColors[0] = frontFace;
		faceColors[1] = rightFace;
		faceColors[2] = leftFace;
		faceColors[3] = topFace;
		faceColors[4] = bottomFace;
		faceColors[5] = backFace;
	}

	void bindFaceColors(const unsigned int shaderID)
	{
		glUniform3fv(glGetUniformLocation(shaderID, "frontColor"), 1, &faceColors[0][0]);
		glUniform3fv(glGetUniformLocation(shaderID, "rightColor"), 1, &faceColors[1][0]);
		glUniform3fv(glGetUniformLocation(shaderID, "leftColor"), 1, &faceColors[2][0]);
		glUniform3fv(glGetUniformLocation(shaderID, "topColor"), 1, &faceColors[3][0]);
		glUniform3fv(glGetUniformLocation(shaderID, "bottomColor"), 1, &faceColors[4][0]);
		glUniform3fv(glGetUniformLocation(shaderID, "backColor"), 1, &faceColors[5][0]);
	}

	void rotatePitch(float degreesPitch)
	{
		worldRotation.x += glm::radians(degreesPitch);
		float theta = worldRotation.x;

		currentPosition = EQuaternion::rotatePitch(currentPosition, glm::radians(degreesPitch));

		localRotation.x = theta - initialWorldRotation.x;
	}

	void rotateYaw(float degreesYaw)
	{
		worldRotation.y += glm::radians(degreesYaw);
		float theta = worldRotation.y;

		currentPosition = EQuaternion::rotateYaw(currentPosition, glm::radians(degreesYaw));

		localRotation.y = theta - initialWorldRotation.y;
	}

	void rotateRoll(float degreesRoll)
	{
		worldRotation.z += glm::radians(degreesRoll);
		float theta = worldRotation.z;

		currentPosition = EQuaternion::rotateRoll(currentPosition, glm::radians(degreesRoll));

		localRotation.z = theta - initialWorldRotation.z;
	}

	void setStartPosition(glm::vec3 position)
	{
		startingPosition = position;
		setCurrentPosition(position);
		calculateMagnitude();
		calculateWorldRotation();
	}

	void setCurrentPosition(glm::vec3 position)
	{
		currentPosition = position;
	}

	glm::vec3 getStartingPosition()
	{
		return startingPosition;
	}

	glm::vec3 getCurrentPosition()
	{
		return currentPosition;
	}

	glm::vec3 getLocalRotation()
	{
		return localRotation;
	}

private:
	glm::vec3 faceColors[6];
	glm::vec3 startingPosition;
	glm::vec3 currentPosition;
	glm::vec3 localRotation;
	glm::vec3 initialWorldRotation;
	glm::vec3 worldRotation;

	float magnitude;

	void calculateMagnitude()
	{
		// Bad, should have separate magnitude for each axis
		magnitude = glm::sqrt(
			startingPosition.x * startingPosition.x +
			startingPosition.y * startingPosition.y +
			startingPosition.z * startingPosition.z
		);
	}

	void calculateWorldRotation()
	{
		calculateWorldPitch();
		calculateWorldYaw();
		calculateWorldRoll();
	}

	void calculateWorldPitch()
	{
		// TODO: Calculate Pitch when rotated onto the Pitch axis (y != 0 && z != 0)
		if (startingPosition.y == 0.0f && startingPosition.z == 0.0f) return;

		glm::vec3 zForward = glm::vec3(0.0f, 0.0f, magnitude);
		float zyFDot = zForward.z * startingPosition.z;

		worldRotation.x = glm::acos(zyFDot / (magnitude * magnitude));

		if (startingPosition.y > 0.0f)
		{
			worldRotation.x *= -1.0f;
		}

		initialWorldRotation.x = worldRotation.x;
	}

	void calculateWorldYaw()
	{
		// TODO: Calculate Yaw when rotated onto the Yaw axis (x != 0 && z != 0)
		if (startingPosition.x == 0.0f && startingPosition.z == 0.0f) return;

		glm::vec3 zForward = glm::vec3(0.0f, 0.0f, magnitude);
		float zxFDot = zForward.z * startingPosition.z;

		worldRotation.y = glm::acos(zxFDot / (magnitude * magnitude));

		if (startingPosition.x < 0.0f)
		{
			worldRotation.y *= -1.0f;
		}

		initialWorldRotation.y = worldRotation.y;
	}

	void calculateWorldRoll()
	{
		// TODO: Calculate Roll when rotated onto the Roll axis (x != 0 && y != 0)
		if (startingPosition.x == 0.0f && startingPosition.y == 0.0f) return;

		glm::vec3 xForward = glm::vec3(magnitude, 0.0f, 0.0f);
		float xyFDot = xForward.x * startingPosition.x;

		worldRotation.z = glm::acos(xyFDot / (magnitude * magnitude));

		if (startingPosition.y < 0.0f)
		{
			worldRotation.z *= -1.0f;
		}

		initialWorldRotation.z = worldRotation.z;
	}
};

#endif