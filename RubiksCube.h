#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <map>
#include <vector>
#include <memory>
#include "Cube.h"
#include "Ray.h"

class RubikStateMachine;

enum RubiksSection
{
	NONE = 0,
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

	bool isRubikCubeSolved();

	
	void initiateRotation(int axis, RubiksSection section);
	void teardownRotation();
	void rotateSectionPercentage(float targetRadians, float dt, int axis);
	void rotateSectionImmediate(float radians, int axis);

	bool isRotationInProgress() const { return mbIsRotating; }
	
	bool isPlayerSelecting() const { return mSelectedCubeID > -1; }
	void setRotationCompleteCallback(PFnOnRotationComplete onComplete) { onRotationComplete = onComplete; }
	void setOnScrambleComplete(PFnOnScrambleComplete onComplete) { onScrambleComplete = onComplete; }

	void processCubeSelection(const Ray& ray, float cameraDirection);
	void processMouseInput(glm::vec2 mousePosition);
	void processCubeReleased();

	std::shared_ptr<RubikStateMachine> getStateMachine() { return mStateMachine; }

	void setLastMousePosition(glm::vec2 mousePosition) { mLastMousePosition = mousePosition; }

private:
	std::shared_ptr<RubikStateMachine> mStateMachine;

	CubeMap mCubeMap;
	float mDisplacement;
	float mErrorMargin;

	bool mbIsRotating = false;
	float mCurrentRotateTime;
	float mRotateCompletionTime;

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

	// Mouse Movement
	glm::vec2 mLastMousePosition = glm::vec2(0.0f);
	glm::vec2 mCurrentMousePosition = glm::vec2(0.0f);

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

	void toPreviousStateImmediate();
	void toPreviousState();
	void updateSectionColliders();

	void clampRotatingCubes();
	float clampCoordinate(float coordinate) const;
	glm::vec3 clampPosition(const glm::vec3& position) const;
};



#endif