#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class OrbitCamera
{
public:

	OrbitCamera(float oDist = 5.0f) : mTargetPosition(glm::vec3(0.0f, 0.0f, 0.0f)), mWorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), mPitch(0.0f), mYaw(0.0f)
	{
		mOrbitDist = oDist;
		calculatePosition();
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	float getYaw() { return mYaw; }

	void processRotationInput(float yawInput, float pitchInput)
	{
		mYaw += yawInput;
		mPitch += pitchInput;
		mPitch = glm::clamp(mPitch, -89.9f, 89.9f);
		calculatePosition();
		updateCameraVectors();
	}

private:
	float mOrbitDist;
	float mPitch;
	float mYaw;
	glm::vec3 mPosition;
	glm::vec3 mTargetPosition;
	glm::vec3 mWorldUp;
	glm::vec3 mFront;
	glm::vec3 mRight;
	glm::vec3 mUp;
	
	void calculatePosition()
	{
		// Rotate pitch vector (0, 0, orbitDist) about x-axis by current pitch converted to radians
		glm::vec3 pitchPos = glm::vec3(0, -glm::sin(glm::radians(mPitch)) * mOrbitDist, glm::cos(glm::radians(mPitch)) * mOrbitDist);
		
		// Rotate yaw vector (0, 0, pitchPos.y) about y-axis by current yaw converted to radians
		glm::vec3 yawPos = glm::vec3(glm::sin(glm::radians(mYaw)) * pitchPos.z, 0, glm::cos(glm::radians(mYaw)) * pitchPos.z);

		// Get x and y from yaw and y from pitch
		mPosition = glm::vec3(yawPos.x, pitchPos.y, yawPos.z);
	}

	void updateCameraVectors()
	{
		mFront = glm::normalize(mTargetPosition - mPosition);
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}
};


#endif