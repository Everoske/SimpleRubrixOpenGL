#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <map>
#include <vector>
#include <memory>
#include "Cube.h"
#include "Ray.h"

enum RubiksSection
{
	FRONT = 1,
	MIDDLE = 2,
	BACK = 3
};

typedef void (*PFnOnRotationComplete)();
typedef void (*PFnOnScrambleComplete)();

class RubiksCube
{
private:
	typedef std::map<int, std::shared_ptr<Cube>> CubeMap;

public:
	RubiksCube(float cubeDisplacement, float floatMargin, float targetTime);
	~RubiksCube() { mCubeMap.clear(); }

	void renderCubes(const unsigned int& cubeVAO, const unsigned int& shaderID) const;
	void update(float deltaTime);

	void startScrambleSmooth(int scrambleCount);
	void scrambleImmediate();
	bool isRubikCubeSolved();

	bool isRotationInProgress() const { return mbIsRotating; }
	bool isPlayerSelecting() const { return mSelectedCubeID > -1; }
	void setRotationCompleteCallback(PFnOnRotationComplete onComplete) { onRotationComplete = onComplete; }
	void setOnScrambleComplete(PFnOnScrambleComplete onComplete) { onScrambleComplete = onComplete; }

	void processCubeSelection(const Ray& ray, float cameraDirection);
	void processCubeReleased();

private:
	CubeMap mCubeMap;
	float mDisplacement;
	float mErrorMargin;

	// Scramble Parameters
	bool mbIsScrambling = false;
	int mScrambleAxis = -1;
	int mTargetScrambleCount = 5;
	int mCurrentScrambleCount = 0;
	float mScrambleTargetRotation = 90.0f;

	bool mbIsRotating = false;
	float mCurrentRotateTime;
	float mRotateCompletionTime;
	RubiksSection mScrambleSection;

	PFnOnRotationComplete onRotationComplete;
	PFnOnScrambleComplete onScrambleComplete;

	// Cube Selection
	int mLastSelectedCubeID = -1;
	int mSelectedCubeID = -1;
	
	// Section Selection and Rotation
	std::vector<int> mSelectedSectionIDs;
	float mPlayerRotationInput = 0.0f;
	bool mbPlayerRotating = false;
	int mSelectedAxis = -1;
	int mSelectedFaceNormal = -1;
	float mCameraDirection = 0.0f;

	void createCubes();

	// Cube/Section Selection
	void selectCube(int selectedID);
	void selectSection();

	void findSectionCubes();
	void findSectionCubes(RubiksSection section, int axis);
	void findSelectedFaceNormal(glm::vec3 collisionPoint);
	void calculateCameraDirection(float cameraDirection);
	void highlightSection();
	void clearSectionCubes();
	void changeSectionCubes();

	// Rotation Methods

	void rotateSectionPercentage(float targetRadians, float dt, int axis);
	void rotateSectionImmediate(float radians, int axis);

	void toPreviousStateImmediate();
	void toPreviousState();
	void updateSectionColliders();

	void clampRotatingCubes();
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;

	// Scramble methods
	void executeScrambleSmooth(float deltaTime);
	void setupScrambleRotation();
	void performSmoothScrambleRotation(float deltaTime);
	void performImmediateScrambleRotation();
};



#endif