#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Quaternion.h"

class Cube
{
private:
	static int mNextValidID;

public:
	Cube(glm::vec3 startPosition, glm::vec3 halfExtents,
		glm::vec3 frontFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 rightFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 leftFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 topFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 bottomFace = glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3 backFace = glm::vec3(0.05f, 0.05f, 0.05f)
	);

	void bindFaceColors(const unsigned int shaderID);

	// Legacy: Should probably remove if no longer needed
	void rotateSmoothX(float radians, float timePercent);
	void rotateSmoothY(float radians, float timePercent);
	void rotateSmoothZ(float radians, float timePercent);

	void rotateXImmediate(float radians);
	void rotateYImmediate(float radians);
	void rotateZImmediate(float radians);

	void rotateXByPercentage(float targetRadians, float dt);
	void rotateYByPercentage(float targetRadians, float dt);
	void rotateZByPercentage(float targetRadians, float dt);

	void toPreviousStateImmediate();
	void updateColliderPosition();

	void setID(int id);
	void setStartPosition(const glm::vec3& position);
	void setCurrentPosition(const glm::vec3& position);
	void setOrientation(const Quaternion& newOrientation);
	void setHighlight(bool highlightCube);

	void initiateTurn();


	int getID() const { return mID; }
	glm::vec3 getHalfExtents() const { return mHalfExtents; }
	glm::vec3 getStartingPosition() const { return mStartingPosition; }
	glm::vec3 getCurrentPosition() const { return mCurrentPosition; }
	glm::vec3 getLastFixedPosition() const { return mLastFixedPosition; }
	Quaternion getOrientation() const { return mOrientation; }
	glm::mat4x4 getTransformationMatrix() const;
	
	bool isInSolvedPositionAndOrientation();

private:
	int mID;
	glm::vec3 mFaceColors[6];
	bool mbHighlighted = false;

	glm::vec3 mHalfExtents;

	glm::vec3 mStartingPosition;
	glm::vec3 mCurrentPosition;
	glm::vec3 mLastFixedPosition;

	Quaternion mOrientation;
	Quaternion mLastFixedOrientation;

	glm::vec3 mScale;
	glm::vec3 mUp;
	glm::vec3 mForward;

	bool mbIsRotating = false;



	void rotateVectors(glm::vec3 axis, float radians);
	void rotateVectors(Quaternion newOrientation);
	float clampNormalScalar(float scalar);
	void recalculateOrientation();
};

#endif