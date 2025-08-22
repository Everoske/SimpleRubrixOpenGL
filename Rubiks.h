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
	Rubiks(float cubeDisplacement, float floatMargin);
	
	void rotateCubesX(RubrikSection section, bool counterClockwise = false);
	void rotateCubesY(RubrikSection section, bool counterClockwise = false);
	void rotateCubesZ(RubrikSection section, bool counterClockwise = false);

	std::vector<Cube> getCubes() { return cubes; }

private:
	std::vector<Cube> cubes;
	float displacement;
	float errorMargin;

	void createCubes();
	float getSectionCoordinate(RubrikSection section) const;
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;
};



#endif