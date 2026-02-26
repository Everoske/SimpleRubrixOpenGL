#include "Rubiks.h"
#include <iostream>

Rubiks::Rubiks(float cubeDisplacement, float floatMargin, float rotationTime) :
	displacement(cubeDisplacement), errorMargin(floatMargin), rotationTime(rotationTime)
{
	createCubes();
	rotatingIndices = std::vector<int>();
	isRotating = false;
}

void Rubiks::rotateCubesSmoothX(RubrikSection section, float deltaTime, bool counterClockwise)
{
	if (!isRotating)
	{
		float xPosition = getSectionCoordinate(section);
		findRotatingIndicesX(xPosition);
		currentTime = 0.0f;
		isRotating = true;
	}

	currentTime += deltaTime;
	float targetRot = counterClockwise ? -90.0f : 90.0f;
	targetRot = glm::radians(targetRot);
	float timePercent = currentTime / rotationTime;
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateSmoothX(targetRot, timePercent);
	}

	if (currentTime >= rotationTime)
	{
		clampRotatingCubes();
		isRotating = false;
		rotatingIndices = std::vector<int>();
	}
}

void Rubiks::rotateCubesSmoothY(RubrikSection section, float deltaTime, bool counterClockwise)
{
	if (!isRotating)
	{
		float yPosition = getSectionCoordinate(section);
		findRotatingIndicesY(yPosition);
		currentTime = 0.0f;
		isRotating = true;
	}

	currentTime += deltaTime;
	float targetRot = counterClockwise ? -90.0f : 90.0f;
	targetRot = glm::radians(targetRot);
	float timePercent = currentTime / rotationTime;
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateSmoothY(targetRot, timePercent);
	}

	if (currentTime >= rotationTime)
	{
		clampRotatingCubes();
		isRotating = false;
		rotatingIndices = std::vector<int>();
	}
}

void Rubiks::rotateCubesSmoothZ(RubrikSection section, float deltaTime, bool counterClockwise)
{
	if (!isRotating)
	{
		float zPosition = getSectionCoordinate(section);
		findRotatingIndicesZ(zPosition);
		currentTime = 0.0f;
		isRotating = true;
	}

	currentTime += deltaTime;
	float targetRot = counterClockwise ? -90.0f : 90.0f;
	targetRot = glm::radians(targetRot);
	float timePercent = currentTime / rotationTime;
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateSmoothZ(targetRot, timePercent);
	}

	if (currentTime >= rotationTime)
	{
		clampRotatingCubes();
		isRotating = false;
		rotatingIndices = std::vector<int>();
	}
}

// TODO: Should perform several successive smooth rotations over multiple frames
//       Should probably have a means of knowing when it starts and end
//       For now, use a random number for Axis and Section
//       Should I include some kind of Update function?
void Rubiks::scrambleSmooth(float deltaTime)
{
	// If Not Scrambling: Do Scramble Setup


	// If Current Rotation Concludes: Start Next Rotation

	// If Last Rotation Concludes: Stop Scramble
}

// TODO: Should perform several successive rotations instantly (no SLERPING!)
//       All rotations should be performed in a single frame
void Rubiks::scrambleImmediate()
{

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

	glm::vec3 cubePositions[27] =
	{
		// Top cubes
		glm::vec3(0.0f, displacement, 0.0f),
		glm::vec3(0.0f, displacement, displacement),
		glm::vec3(0.0f, displacement, -displacement),
		glm::vec3(displacement, displacement, 0.0f),
		glm::vec3(displacement, displacement, displacement),
		glm::vec3(displacement, displacement, -displacement),
		glm::vec3(-displacement, displacement, 0.0f),
		glm::vec3(-displacement, displacement, displacement),
		glm::vec3(-displacement, displacement, -displacement),

		// Middle cubes
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, displacement),
		glm::vec3(0.0f, 0.0f, -displacement),
		glm::vec3(displacement, 0.0f, 0.0f),
		glm::vec3(displacement, 0.0f, displacement),
		glm::vec3(displacement, 0.0f, -displacement),
		glm::vec3(-displacement, 0.0f, 0.0f),
		glm::vec3(-displacement, 0.0f, displacement),
		glm::vec3(-displacement, 0.0f, -displacement),

		// Bottom cubes
		// Top cubes
		glm::vec3(0.0f, -displacement, 0.0f),
		glm::vec3(0.0f, -displacement, displacement),
		glm::vec3(0.0f, -displacement, -displacement),
		glm::vec3(displacement, -displacement, 0.0f),
		glm::vec3(displacement, -displacement, displacement),
		glm::vec3(displacement, -displacement, -displacement),
		glm::vec3(-displacement, -displacement, 0.0f),
		glm::vec3(-displacement, -displacement, displacement),
		glm::vec3(-displacement, -displacement, -displacement),
	};

	cubes.push_back(Cube(Black, Black, Black, Green, Black, Black));
	cubes.push_back(Cube(Yellow, Black, Black, Green, Black, Black));
	cubes.push_back(Cube(Black, Black, Black, Green, Black, White));
	cubes.push_back(Cube(Black, Red, Black, Green, Black, Black));
	cubes.push_back(Cube(Yellow, Red, Black, Green, Black, Black));
	cubes.push_back(Cube(Black, Red, Black, Green, Black, White));
	cubes.push_back(Cube(Black, Black, Orange, Green, Black, Black));
	cubes.push_back(Cube(Yellow, Black, Orange, Green, Black, Black));
	cubes.push_back(Cube(Black, Black, Orange, Green, Black, White));

	cubes.push_back(Cube(Black, Black, Black, Black, Black, Black));
	cubes.push_back(Cube(Yellow, Black, Black, Black, Black, Black));
	cubes.push_back(Cube(Black, Black, Black, Black, Black, White));
	cubes.push_back(Cube(Black, Red, Black, Black, Black, Black));
	cubes.push_back(Cube(Yellow, Red, Black, Black, Black, Black));
	cubes.push_back(Cube(Black, Red, Black, Black, Black, White));
	cubes.push_back(Cube(Black, Black, Orange, Black, Black, Black));
	cubes.push_back(Cube(Yellow, Black, Orange, Black, Black, Black));
	cubes.push_back(Cube(Black, Black, Orange, Black, Black, White));

	cubes.push_back(Cube(Black, Black, Black, Black, Blue, Black));
	cubes.push_back(Cube(Yellow, Black, Black, Black, Blue, Black));
	cubes.push_back(Cube(Black, Black, Black, Black, Blue, White));
	cubes.push_back(Cube(Black, Red, Black, Black, Blue, Black));
	cubes.push_back(Cube(Yellow, Red, Black, Black, Blue, Black));
	cubes.push_back(Cube(Black, Red, Black, Black, Blue, White));
	cubes.push_back(Cube(Black, Black, Orange, Black, Blue, Black));
	cubes.push_back(Cube(Yellow, Black, Orange, Black, Blue, Black));
	cubes.push_back(Cube(Black, Black, Orange, Black, Blue, White));

	for (int i = 0; i < 27; i++)
	{
		cubes.at(i).setStartPosition(cubePositions[i]);
	}
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

void Rubiks::findRotatingIndicesX(float xPosition)
{
	for (int i = 0; i < 27; i++)
	{
		if (cubes.at(i).getCurrentPosition().x == xPosition)
		{
			rotatingIndices.push_back(i);
		}
	}
	//std::cout << "Rotating " << rotatingIndices.size() << " Cubes" << std::endl;
}

void Rubiks::findRotatingIndicesY(float yPosition)
{
	for (int i = 0; i < 27; i++)
	{
		if (cubes.at(i).getCurrentPosition().y == yPosition)
		{
			rotatingIndices.push_back(i);
		}
	}
	//std::cout << "Rotating " << rotatingIndices.size() << " Cubes" << std::endl;
}

void Rubiks::findRotatingIndicesZ(float zPosition)
{
	for (int i = 0; i < 27; i++)
	{
		if (cubes.at(i).getCurrentPosition().z == zPosition)
		{
			rotatingIndices.push_back(i);
		}
	}
	//std::cout << "Rotating " << rotatingIndices.size() << " Cubes" << std::endl;
}

// TODO: Remove and move logic into Cube?
void Rubiks::clampRotatingCubes()
{
	for (int i : rotatingIndices)
	{
		glm::vec3 currentPosition = cubes.at(i).getCurrentPosition();
		cubes.at(i).setCurrentPosition(clampPosition(currentPosition));
	}
}

// TODO: Move this into the Cube class?
// After a full rotation, ensure cube's position is clamped to an exact displacement
float Rubiks::clampCoordinate(float coordinate) const
{
	if (coordinate > -errorMargin && coordinate < errorMargin)
		return 0.0f;
	if (coordinate > errorMargin && coordinate < displacement || coordinate > displacement)
		return displacement;
	if (coordinate < -errorMargin && coordinate > -displacement || coordinate < -displacement)
		return -displacement;
	return coordinate;
}

// TODO: Move this into the Cube class?
glm::vec3 Rubiks::clampPosition(const glm::vec3& position) const
{
	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	glm::vec3 newPos = glm::vec3(clampCoordinate(position.x), clampCoordinate(position.y), clampCoordinate(position.z));
	std::cout << newPos.x << ", " << newPos.y << ", " << newPos.z << std::endl;
	return newPos;
}

void Rubiks::startScrambleRotation(int axis, int sectionInt, float deltaTime, bool counterClockwise)
{
	RubrikSection section = static_cast<RubrikSection>(sectionInt);
	if (axis == 1)
		rotateCubesSmoothX(section, deltaTime, counterClockwise);
	else if (axis == 2)
		rotateCubesSmoothY(section, deltaTime, counterClockwise);
	else
		rotateCubesSmoothZ(section, deltaTime, counterClockwise);
}
