#ifndef RUBIKS_H
#define RUBIKS_H

#include "Cube.h"
#include <array>

enum RubrikSection
{
	FRONT = 1,
	MIDDLE = 2,
	BACK = 3
};

class Rubiks
{
public:
	Rubiks(float cubeDisplacement, float floatMargin);
	
	void rotateCubesX(RubrikSection section, bool counterClockwise);
	void rotateCubesY(RubrikSection section, bool counterClockwise);
	void rotateCubesZ(RubrikSection section, bool counterClockwise);

private:
	std::array<Cube, 27> cubes;
	float displacement;
	float errorMargin;

	void createCubes();
	float getSectionCoordinate(RubrikSection section) const;
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;
};



#endif