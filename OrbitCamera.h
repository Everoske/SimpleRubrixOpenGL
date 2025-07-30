#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class OrbitCamera
{
public:

	OrbitCamera(float oDist = 5.0f) : targetPosition(glm::vec3(0.0f, 0.0f, 0.0f)), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), pitch(0.0f), yaw(0.0f)
	{
		orbitDist = oDist;
		calculatePosition();
	}

	glm::mat4 getViewMatrix()
	{
		//std::cout << "Position: " << position.x << "," << position.y << "," << position.z << std::endl;
		//std::cout << "Target Pos: " << targetPosition.x << "," << targetPosition.y << "," << targetPosition.z << std::endl;
		//std::cout << "World Up: " << worldUp.x << "," << worldUp.y << "," << worldUp.z << std::endl;

		return glm::lookAt(position, targetPosition, worldUp);
	}

	void processRotationInput(float pitchInput, float yawInput)
	{
		pitch += pitchInput;
		pitch = glm::clamp(pitch, -90.0f, 90.0f);
		yaw += yawInput;
		calculatePosition();
	}

private:
	float orbitDist;
	float pitch;
	float yaw;
	glm::vec3 position;
	glm::vec3 targetPosition;
	glm::vec3 worldUp;
	
	void calculatePosition()
	{
		// Rotate pitch vector (0, 0, orbitDist) about x-axis by current pitch converted to radians
		glm::vec3 pitchPos = glm::vec3(0, -glm::sin(glm::radians(pitch)) * orbitDist, glm::cos(glm::radians(pitch)) * orbitDist);
		
		// Rotate yaw vector (0, 0, pitchPos.y) about y-axis by current yaw converted to radians
		glm::vec3 yawPos = glm::vec3(glm::sin(glm::radians(yaw)) * pitchPos.z, 0, glm::cos(glm::radians(yaw)) * pitchPos.z);

		// Get x and y from yaw and y from pitch
		position = glm::vec3(yawPos.x, pitchPos.y, yawPos.z);
	}
};


#endif