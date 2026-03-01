#include "Rubiks.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

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
		if (onRotationComplete)
			onRotationComplete();
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
		if (onRotationComplete)
			onRotationComplete();
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
		if (onRotationComplete)
			onRotationComplete();
	}
}

void Rubiks::scrambleSmooth(float deltaTime)
{
	// If Not Scrambling: Do Scramble Setup
	if (!isScrambling)
	{
		currentScrambleRotations = 0;
		setupScrambleRotation();
		isScrambling = true;
		performSmoothScrambleRotation(deltaTime);
		return;
	}

	if (isRotating)
	{
		performSmoothScrambleRotation(deltaTime);
	}
	else
	{
		currentScrambleRotations++;
		if (currentScrambleRotations < totalScrambleRotations)
		{
			setupScrambleRotation();
			isScrambling = true;
			performSmoothScrambleRotation(deltaTime);
		}
		else
		{
			isScrambling = false;
			onScrambleComplete();
		}
	}
}

void Rubiks::scrambleImmediate()
{
	if (!isScrambling)
	{
		currentScrambleRotations = 0;
	}
	else
	{
		isScrambling = false;
	}

	int remaining = isRotating ? currentScrambleRotations : currentScrambleRotations + 1;
	for (remaining; remaining < totalScrambleRotations; remaining++)
	{
		setupScrambleRotation();
		performImmediateScrambleRotation();
	}

	if (onScrambleComplete)
		onScrambleComplete();
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

glm::vec3 Rubiks::clampPosition(const glm::vec3& position) const
{
	glm::vec3 newPos = glm::vec3(clampCoordinate(position.x), clampCoordinate(position.y), clampCoordinate(position.z));
	return newPos;
}

void Rubiks::rotateCubesX(RubrikSection section)
{
	if (!isRotating)
	{
		float xPosition = getSectionCoordinate(section);
		findRotatingIndicesX(xPosition);
	}
	else
	{
		isRotating = false;
	}

	constexpr float targetRot = glm::radians(90.0f);
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateXImmediate(targetRot);
	}

	clampRotatingCubes();
	rotatingIndices = std::vector<int>();
}

void Rubiks::rotateCubesY(RubrikSection section)
{
	if (!isRotating)
	{
		float yPosition = getSectionCoordinate(section);
		findRotatingIndicesY(yPosition);
	}
	else
	{
		isRotating = false;
	}

	constexpr float targetRot = glm::radians(90.0f);
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateYImmediate(targetRot);
	}

	clampRotatingCubes();
	rotatingIndices = std::vector<int>();
}

void Rubiks::rotateCubesZ(RubrikSection section)
{
	if (!isRotating)
	{
		float zPosition = getSectionCoordinate(section);
		findRotatingIndicesZ(zPosition);
	}
	else
	{
		isRotating = false;
	}

	constexpr float targetRot = glm::radians(90.0f);
	for (int i : rotatingIndices)
	{
		cubes.at(i).rotateZImmediate(targetRot);
	}

	clampRotatingCubes();
	rotatingIndices = std::vector<int>();
}

void Rubiks::setupScrambleRotation()
{
	srand(time(0));
	int newAxis = (rand() % 3) + 1;
	int newSection = (rand() % 3) + 1;
	int oldSection = (int) scrambleSection;

	if (newAxis == scrambleAxis && newSection == oldSection)
	{
		if (newAxis == newSection)
		{
			newAxis = ((newAxis + 4) % 3) + 1;
			newSection = ((newSection + 2) % 3) + 1;
		}
		else if (newAxis % 2 > 0 && newSection % 2 > 0)
		{
			newAxis = ((newAxis + 4) % 3) + 1;
			newSection = ((newSection + 4) % 3) + 1;
		}
		else if (newAxis % 2 == 0 && newSection % 2 == 0)
		{
			newAxis = ((newAxis + 2) % 3) + 1;
			newSection = ((newSection + 2) % 3) + 1;
		}
		else
		{
			int temp = newAxis;
			newAxis = newSection;
			newSection = temp;
		}
	}

	scrambleAxis = newAxis;
	scrambleSection = static_cast<RubrikSection>(newSection);
}

void Rubiks::performSmoothScrambleRotation(float deltaTime)
{
	switch (scrambleAxis)
	{
	case 1:
		rotateCubesSmoothX(scrambleSection, deltaTime);
		break;
	case 2:
		rotateCubesSmoothY(scrambleSection, deltaTime);
		break;
	case 3:
		rotateCubesSmoothZ(scrambleSection, deltaTime);
		break;
	}
}

void Rubiks::performImmediateScrambleRotation()
{
	switch (scrambleAxis)
	{
	case 1:
		rotateCubesX(scrambleSection);
		break;
	case 2:
		rotateCubesY(scrambleSection);
		break;
	case 3:
		rotateCubesZ(scrambleSection);
		break;
	}
}
