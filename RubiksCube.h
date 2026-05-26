#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <map>
#include <vector>
#include <memory>
#include "Cube.h"

enum RubrikSection
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
	
	void highlightSelectedCubes(int axis, RubrikSection section);
	void rotateCubesSmooth(int axis, RubrikSection section, float deltaTime, bool counterClockwise = false);
	void rotateCubesSmoothX(RubrikSection section, float deltaTime, bool counterClockwise = false);
	void rotateCubesSmoothY(RubrikSection section, float deltaTime, bool counterClockwise = false);
	void rotateCubesSmoothZ(RubrikSection section, float deltaTime, bool counterClockwise = false);

	void scrambleSmooth(float deltaTime);
	void scrambleImmediate();
	bool isRubikCubeSolved();

	bool isRotationInProgress() const { return mbIsRotating; }
	void setRotationCompleteCallback(PFnOnRotationComplete onComplete) { onRotationComplete = onComplete; }
	void setOnScrambleComplete(PFnOnScrambleComplete onComplete) { onScrambleComplete = onComplete; }

private:
	CubeMap mCubeMap;
	std::vector<int> rotatingIndices;
	std::vector<int> selectedIndices;
	float mDisplacement;
	float mErrorMargin;

	bool mbIsRotating;
	float mCurrentRotateTime;
	float mRotateCompletionTime;
	bool mbIsScrambling = false;
	int mScrambleAxis = -1;
	int mTotalScrambleRotations = 5;
	int mCurrentScrambleRotations = 0;
	RubrikSection mScrambleSection;

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

	void createCubes();

	void findSectionCubes();

	void clampRotatingCubes();
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;

	void rotateCubesX(RubrikSection section);
	void rotateCubesY(RubrikSection section);
	void rotateCubesZ(RubrikSection section);

	void setupScrambleRotation();
	void performSmoothScrambleRotation(float deltaTime);
	void performImmediateScrambleRotation();
};



#endif