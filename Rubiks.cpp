#include "Rubiks.h"

Rubiks::Rubiks(float cubeDisplacement, float floatMargin) :
	displacement(cubeDisplacement), errorMargin(floatMargin)
{
	createCubes();
}

void Rubiks::rotateCubesX(RubrikSection section, bool counterClockwise)
{
	float rotation = counterClockwise ? -90.0f : 90.0f;
	float x = getSectionCoordinate(section);

	for (int i = 0; i < 27; i++)
	{
		if (cubes[i].getCurrentPosition().x == x)
		{
			cubes[i].rotatePitch(rotation);
			glm::vec3 clampedPosition = clampPosition(cubes[i].getCurrentPosition());
			cubes[i].setCurrentPosition(clampedPosition);
		}
	}
}

void Rubiks::rotateCubesY(RubrikSection section, bool counterClockwise)
{
	float rotation = counterClockwise ? -90.0f : 90.0f;
	float y = getSectionCoordinate(section);

	for (int i = 0; i < 27; i++)
	{
		if (cubes[i].getCurrentPosition().y == y)
		{
			cubes[i].rotateYaw(rotation);
			glm::vec3 clampedPosition = clampPosition(cubes[i].getCurrentPosition());
			cubes[i].setCurrentPosition(clampedPosition);
		}
	}
}

void Rubiks::rotateCubesZ(RubrikSection section, bool counterClockwise)
{
	float rotation = counterClockwise ? -90.0f : 90.0f;
	float z = getSectionCoordinate(section);

	for (int i = 0; i < 27; i++)
	{
		if (cubes[i].getCurrentPosition().z == z)
		{
			cubes[i].rotateRoll(rotation);
			glm::vec3 clampedPosition = clampPosition(cubes[i].getCurrentPosition());
			cubes[i].setCurrentPosition(clampedPosition);
		}
	}
}

void Rubiks::createCubes()
{
	const glm::vec3 Green = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 Red = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 Orange = glm::vec3(1.0f, 0.5f, 0.0f);
	const glm::vec3 Yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	const glm::vec3 White = glm::vec3(1.0f);
	const glm::vec3 Blue = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 Black = glm::vec3(0.05f, 0.05f, 0.05f);

	cubes = {
		Cube(Black, Black, Black, Green, Black, Black),
		Cube(Yellow, Black, Black, Green, Black, Black),
		Cube(Black, Black, Black, Green, Black, White),
		Cube(Black, Red, Black, Green, Black, Black),
		Cube(Yellow, Red, Black, Green, Black, Black),
		Cube(Black, Red, Black, Green, Black, White),
		Cube(Black, Black, Orange, Green, Black, Black),
		Cube(Yellow, Black, Orange, Green, Black, Black),
		Cube(Black, Black, Orange, Green, Black, White),

		Cube(Black, Black, Black, Black, Black, Black),
		Cube(Yellow, Black, Black, Black, Black, Black),
		Cube(Black, Black, Black, Black, Black, White),
		Cube(Black, Red, Black, Black, Black, Black),
		Cube(Yellow, Red, Black, Black, Black, Black),
		Cube(Black, Red, Black, Black, Black, White),
		Cube(Black, Black, Orange, Black, Black, Black),
		Cube(Yellow, Black, Orange, Black, Black, Black),
		Cube(Black, Black, Orange, Black, Black, White),

		Cube(Black, Black, Black, Black, Blue, Black),
		Cube(Yellow, Black, Black, Black, Blue, Black),
		Cube(Black, Black, Black, Black, Blue, White),
		Cube(Black, Red, Black, Black, Blue, Black),
		Cube(Yellow, Red, Black, Black, Blue, Black),
		Cube(Black, Red, Black, Black, Blue, White),
		Cube(Black, Black, Orange, Black, Blue, Black),
		Cube(Yellow, Black, Orange, Black, Blue, Black),
		Cube(Black, Black, Orange, Black, Blue, White)
	};
}

float Rubiks::getSectionCoordinate(RubrikSection section) const
{
	switch (section)
	{
	case RubrikSection::FRONT:
		return displacement;
	case RubrikSection::BACK:
		return -displacement;
	}

	return 0.0f;
}

// After a full rotation, ensure cube's position is clamped to an exact displacement
float Rubiks::clampCoordinate(float coordinate) const
{
	if (coordinate > -errorMargin && coordinate < errorMargin)
		return 0.0f;
	if (coordinate > errorMargin && coordinate < displacement || coordinate > displacement - errorMargin)
		return displacement;
	if (coordinate < -errorMargin && coordinate > -displacement || coordinate < -displacement + errorMargin)
		return -displacement;

	return coordinate;
}

glm::vec3 Rubiks::clampPosition(const glm::vec3& position) const
{
	return glm::vec3(clampCoordinate(position.x), clampCoordinate(position.y), clampCoordinate(position.z));
}
