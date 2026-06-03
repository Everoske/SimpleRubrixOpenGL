#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "CollisionSystem.h"
#include "RubiksCube.h"

int Cube::mNextValidID = 0;

Cube::Cube(glm::vec3 startPosition, glm::vec3 halfExtents,	
	glm::vec3 frontFace,
	glm::vec3 rightFace,
	glm::vec3 leftFace,
	glm::vec3 topFace,
	glm::vec3 bottomFace,
	glm::vec3 backFace
) :
	mStartingPosition{ startPosition },
	mHalfExtents{ halfExtents },
	mCurrentPosition{ startPosition },
	mOrientation{ Quaternion() },
	mScale{ halfExtents },
	mUp{ glm::vec3(0.0f, 1.0f, 0.0f) },
	mForward{ glm::vec3(0.0f, 0.0f, -1.0f) }
{
	setID(mNextValidID);
	mFaceColors[0] = frontFace;
	mFaceColors[1] = rightFace;
	mFaceColors[2] = leftFace;
	mFaceColors[3] = topFace;
	mFaceColors[4] = bottomFace;
	mFaceColors[5] = backFace;

	mLastFixedOrientation = mOrientation;
	mLastFixedPosition = mCurrentPosition;
	
	// TODO: Need to disable collision for middle cube
	CollisionSystem::Instance()->generateAABBCollider(mID, mHalfExtents, mCurrentPosition);
}

void Cube::bindFaceColors(const unsigned int shaderID)
{
	glUniform3fv(glGetUniformLocation(shaderID, "frontColor"), 1, &mFaceColors[0][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "rightColor"), 1, &mFaceColors[1][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "leftColor"), 1, &mFaceColors[2][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "topColor"), 1, &mFaceColors[3][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "bottomColor"), 1, &mFaceColors[4][0]);
	glUniform3fv(glGetUniformLocation(shaderID, "backColor"), 1, &mFaceColors[5][0]);
	glUniform1i(glGetUniformLocation(shaderID, "highlight"), mbHighlighted);
}

void Cube::rotateSmoothX(float radians, float timePercent)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	glm::vec3 rightVector = glm::cross(mUp, mForward);
	rightVector = glm::vec3(clampNormalScalar(rightVector.x), clampNormalScalar(rightVector.y), clampNormalScalar(rightVector.z));
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(1.0f, 0.0f, 0.0f), mLastFixedPosition, radians);
		setCurrentPosition(newPosition);
		mbIsRotating = false;
		return;
	}
	
	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(1.0f, 0.0f, 0.0f), radians, timePercent);
	mCurrentPosition = newPosition;
}

void Cube::rotateSmoothY(float radians, float timePercent)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(mUp, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, -1.0f, 0.0f), mLastFixedPosition, radians);
		setCurrentPosition(newPosition);
		mbIsRotating = false;
		return;
	}

	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(0.0f, -1.0f, 0.0f), radians, timePercent);
	mCurrentPosition = newPosition;
}

void Cube::rotateSmoothZ(float radians, float timePercent)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(mForward, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	if (timePercent >= 1.0f)
	{
		setOrientation(targetOrientation);
		glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, 0.0f, 1.0f), mLastFixedPosition, radians);
		setCurrentPosition(newPosition);
		mbIsRotating = false;
		return;
	}

	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, timePercent);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(0.0f, 0.0f, 1.0f), radians, timePercent);
	mCurrentPosition = newPosition;
}

void Cube::rotateXImmediate(float radians)
{
	if (mbIsRotating)
	{
		mbIsRotating = false;
	}
	else
	{
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	glm::vec3 rightVector = glm::cross(mUp, mForward);
	rightVector = glm::vec3(clampNormalScalar(rightVector.x), clampNormalScalar(rightVector.y), clampNormalScalar(rightVector.z));
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(1.0f, 0.0f, 0.0f), mLastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::rotateYImmediate(float radians)
{
	if (mbIsRotating)
	{
		mbIsRotating = false;
	}
	else
	{
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(mUp, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, -1.0f, 0.0f), mLastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::rotateZImmediate(float radians)
{
	if (mbIsRotating)
	{
		mbIsRotating = false;
	}
	else
	{
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(-mForward, radians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;
	setOrientation(targetOrientation);
	glm::vec3 newPosition = Quaternion::rotatePoint(glm::vec3(0.0f, 0.0f, -1.0f), mLastFixedPosition, radians);
	setCurrentPosition(newPosition);
}

void Cube::rotateXByPercentage(float targetRadians, float dt)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	glm::vec3 rightVector = glm::cross(mUp, mForward);
	rightVector = glm::vec3(clampNormalScalar(rightVector.x), clampNormalScalar(rightVector.y), clampNormalScalar(rightVector.z));
	Quaternion targetRotator = Quaternion::getRotationQuaternion(rightVector, targetRadians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, dt);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(1.0f, 0.0f, 0.0f), targetRadians, dt);
	mCurrentPosition = newPosition;
}

void Cube::rotateYByPercentage(float targetRadians, float dt)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(mUp, targetRadians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, dt);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(0.0f, -1.0f, 0.0f), targetRadians, dt);
	mCurrentPosition = newPosition;
}

void Cube::rotateZByPercentage(float targetRadians, float dt)
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}

	Quaternion targetRotator = Quaternion::getRotationQuaternion(-mForward, targetRadians);
	Quaternion targetOrientation = targetRotator * mLastFixedOrientation;

	mOrientation = Quaternion::slerp(mLastFixedOrientation, targetOrientation, dt);
	glm::vec3 newPosition = Quaternion::slerpPoint(mLastFixedPosition, glm::vec3(0.0f, 0.0f, -1.0f), targetRadians, dt);
	mCurrentPosition = newPosition;
}

void Cube::toPreviousStateImmediate()
{
	mOrientation = mLastFixedOrientation;
	mCurrentPosition = mLastFixedPosition;
}

void Cube::updateColliderPosition()
{
	CollisionSystem::Instance()->updateColliderPosition(mID, mCurrentPosition);
}

void Cube::setID(int id)
{
	assert(id >= mNextValidID);
	mID = id;
	mNextValidID = mID + 1;
}

void Cube::setStartPosition(const glm::vec3& position)
{
	mStartingPosition = position;
	setCurrentPosition(position);
}

void Cube::setCurrentPosition(const glm::vec3& position)
{
	mCurrentPosition = position;
}

void Cube::setOrientation(const Quaternion& newOrientation)
{
	mOrientation = newOrientation;
	rotateVectors(mOrientation);
}

void Cube::setHighlight(bool highlightCube)
{
	mbHighlighted = highlightCube;
}

void Cube::initiateTurn()
{
	if (!mbIsRotating)
	{
		mbIsRotating = true;
		mLastFixedPosition = mCurrentPosition;
		mLastFixedOrientation = mOrientation;
	}
}

glm::mat4x4 Cube::getTransformationMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, mCurrentPosition) * mOrientation.toRotationMatrix();
	model = glm::scale(model, mScale);
	return model;
}

bool Cube::isInSolvedPositionAndOrientation()
{
	return mCurrentPosition == mStartingPosition && mUp.y == 1.0f && mForward.z == -1.0f;
}

// Try to clamp vectors???
void Cube::rotateVectors(glm::vec3 axis, float radians)
{
	mUp = glm::normalize(Quaternion::rotatePoint(axis, mUp, radians));
	mForward = glm::normalize(Quaternion::rotatePoint(axis, mForward, radians));
}

// Try to clamp vectors??
void Cube::rotateVectors(Quaternion newOrientation)
{
	mUp = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 1.0f, 0.0f)));
	mForward = glm::normalize(Quaternion::rotatePoint(newOrientation, glm::vec3(0.0f, 0.0f, -1.0f)));
	mUp = glm::vec3(clampNormalScalar(mUp.x), clampNormalScalar(mUp.y), clampNormalScalar(mUp.z));
	mForward = glm::vec3(clampNormalScalar(mForward.x), clampNormalScalar(mForward.y), clampNormalScalar(mForward.z));
	recalculateOrientation();
}

float Cube::clampNormalScalar(float scalar)
{
	if (scalar > 0.5f)
		return 1.0f;
	if (scalar < -0.5f)
		return -1.0f;
	
	return 0.0f;
}

void Cube::recalculateOrientation()
{
	if (mUp.y == 1.0f && mForward.z == -1.0f)
	{
		//std::cout << "In Original Position" << std::endl;
		mOrientation = Quaternion();
	}
	else if (mUp.y == 1.0f)
	{
		//std::cout << "Up in original Position" << std::endl;
		float yAngle = 0.0f;
		if (mForward.z == 1.0f)
			yAngle = glm::radians(180.0f);
		else if (mForward.x == 1.0f)
			yAngle = glm::radians(-90.0f);
		else if (mForward.x == -1.0f)
			yAngle = glm::radians(90.0f);
		Quaternion rotator = Quaternion::getRotationQuaternion(mUp, yAngle);
		mOrientation = rotator * Quaternion();

	}
	else if (mForward.z == -1.0f)
	{
		//std::cout << "Forward in original Position" << std::endl;
		float zAngle = 0.0f;
		if (mUp.y == -1.0f)
			zAngle = glm::radians(180.0f);
		else if (mUp.x == 1.0f)
			zAngle = glm::radians(90.0f);
		else if (mUp.x == -1.0f)
			zAngle = glm::radians(-90.0f);
		Quaternion rotator = Quaternion::getRotationQuaternion(mForward, zAngle);
		mOrientation = rotator * Quaternion();
	}
}
