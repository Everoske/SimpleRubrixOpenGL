#ifndef RUBIKS_H
#define RUBIKS_H

#include "Cube.h"
#include <vector>

enum RubrikSection
{
	FRONT = 1,
	MIDDLE = 2,
	BACK = 3
};

class Rubiks
{
public:
	Rubiks(float cubeDisplacement, float floatMargin, float targetTime);
	
	void rotateCubesSmoothX(RubrikSection section, float deltaTime, bool counterClockwise = false);

	std::vector<Cube> getCubes() { return cubes; }
	bool isRotationInProgress() const { return isRotating; }

private:
	std::vector<Cube> cubes;
	std::vector<int> rotatingIndices;
	float displacement;
	float errorMargin;

	bool isRotating;
	float currentTime;
	float rotationTime;

	void createCubes();
	float getSectionCoordinate(RubrikSection section) const;
	void findRotatingIndicesX(float xPosition);
	void clampRotatingCubes();
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;
};



#endif