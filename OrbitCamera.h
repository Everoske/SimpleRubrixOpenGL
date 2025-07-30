#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Orbit around the center of the scene at a fixed distance
// Rotate pitch but clamp at 90.0f and -90.0f
// 90.0f Pitch: Staring straight down at Rubrix cube
// -90.0f Pitch: Staring straight up at Rubrix cube
// Rotate yaw without clamp
// Avoid gimbal lock
class OrbitCamera
{
public:

	void processRotationInput(float pitchInput, float yawInput)
	{
		pitch += glm::clamp(pitchInput, -90.0f, 90.0f);
		yaw += yawInput;
	}

private:
	float orbitDist;
	float pitch;
	float yaw;
	glm::vec3 position;
	
	void calculatePosition()
	{
		// Rotate pitch vector (0, 0, orbitDist) about x-axis by current pitch converted to radians
		glm::vec3 pitchPos = glm::vec3(0, -glm::sin(glm::radians(pitch)) * orbitDist, glm::cos(glm::radians(pitch)) * orbitDist);
		
		// Rotate yaw vector (0, 0, pitchPos.y) about y-axis by current yaw converted to radians
		glm::vec3 yawPos = glm::vec3(glm::sin(glm::radians(yaw)) * pitchPos.z, 0, glm::cos(glm::radians(yaw)) * pitchPos.z);

		// Add vectors to get position of camera at current yaw and pitch
		position = pitchPos + yawPos;
	}
};


#endif