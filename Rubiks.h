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

typedef void (*PFnOnRotationComplete)();
typedef void (*PFnOnScrambleComplete)();

class Rubiks
{
public:
	Rubiks(float cubeDisplacement, float floatMargin, float targetTime);
	
	void rotateCubesSmoothX(RubrikSection section, float deltaTime, bool counterClockwise = false);
	void rotateCubesSmoothY(RubrikSection section, float deltaTime, bool counterClockwise = false);
	void rotateCubesSmoothZ(RubrikSection section, float deltaTime, bool counterClockwise = false);

	void scrambleSmooth(float deltaTime);
	void scrambleImmediate();

	std::vector<Cube> getCubes() { return cubes; }
	bool isRotationInProgress() const { return isRotating; }
	void setRotationCompleteCallback(PFnOnRotationComplete onComplete) { onRotationComplete = onComplete; }
	void setOnScrambleComplete(PFnOnScrambleComplete onComplete) { onScrambleComplete = onComplete; }

private:
	std::vector<Cube> cubes;
	std::vector<int> rotatingIndices;
	float displacement;
	float errorMargin;

	bool isRotating;
	float currentTime;
	float rotationTime;
	bool isScrambling = false;
	int scrambleAxis = -1;
	int totalScrambleRotations = 40;
	int currentScrambleRotations = 0;
	RubrikSection scrambleSection;

	PFnOnRotationComplete onRotationComplete;
	PFnOnScrambleComplete onScrambleComplete;

	void createCubes();
	float getSectionCoordinate(RubrikSection section) const;
	void findRotatingIndicesX(float xPosition);
	void findRotatingIndicesY(float yPosition);
	void findRotatingIndicesZ(float zPosition);
	void clampRotatingCubes();
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;

	void startScrambleRotation();
	void performScrambleRotation(float deltaTime);
};



#endif