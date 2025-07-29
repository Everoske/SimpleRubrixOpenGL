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

private:

};


#endif