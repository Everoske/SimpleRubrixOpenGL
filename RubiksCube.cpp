#include "RubiksCube.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "CollisionSystem.h"

RubiksCube::RubiksCube(float cubeDisplacement, float floatMargin, float rotationTime) :
	mDisplacement(cubeDisplacement), mErrorMargin(floatMargin), mRotateCompletionTime(rotationTime)
{
	createCubes();
}

void RubiksCube::renderCubes(const unsigned int& cubeVAO, const unsigned int& shaderID) const
{
	CubeMap::const_iterator it;

	for (it = mCubeMap.begin(); it != mCubeMap.end(); it++)
	{
		glBindVertexArray(cubeVAO);
		glm::mat4x4 model = it->second->getTransformationMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);
		it->second->bindFaceColors(shaderID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void RubiksCube::update(float deltaTime)
{
	if (mbIsScrambling)
		executeScrambleSmooth(deltaTime);
}

void RubiksCube::startScrambleSmooth(int scrambleCount)
{
	mCurrentScrambleCount = 0;
	mTargetScrambleCount = scrambleCount;
	setupScrambleRotation();
	mbIsScrambling = true;
}

void RubiksCube::scrambleImmediate()
{
	if (!mbIsScrambling)
	{
		mCurrentScrambleCount = 0;
	}
	else
	{
		mbIsScrambling = false;
	}

	int remaining = mbIsRotating ? mCurrentScrambleCount : mCurrentScrambleCount + 1;
	for (remaining; remaining < mTargetScrambleCount; remaining++)
	{
		setupScrambleRotation();
		performImmediateScrambleRotation();
	}

	if (onScrambleComplete)
		onScrambleComplete();
}

bool RubiksCube::isRubikCubeSolved()
{
	CubeMap::const_iterator it;

	for (it = mCubeMap.begin(); it != mCubeMap.end(); it++)
	{
		if (!it->second->isInSolvedPositionAndOrientation())
			return false;
	}

	return true;
}

void RubiksCube::createCubes()
{
const glm::vec3 Green = glm::vec3(0.0f, 0.6f, 0.0f);
const glm::vec3 Red = glm::vec3(0.6f, 0.0f, 0.0f);
const glm::vec3 Orange = glm::vec3(0.6f, 0.3f, 0.0f);
const glm::vec3 Yellow = glm::vec3(0.6f, 0.6f, 0.0f);
const glm::vec3 White = glm::vec3(0.6f);
const glm::vec3 Blue = glm::vec3(0.0f, 0.0f, 0.6f);
const glm::vec3 Black = glm::vec3(0.05f, 0.05f, 0.05f);

	glm::vec3 cubePositions[27] =
	{
		// Top cubes
		glm::vec3(0.0f, mDisplacement, 0.0f),
		glm::vec3(0.0f, mDisplacement, mDisplacement),
		glm::vec3(0.0f, mDisplacement, -mDisplacement),
		glm::vec3(mDisplacement, mDisplacement, 0.0f),
		glm::vec3(mDisplacement, mDisplacement, mDisplacement),
		glm::vec3(mDisplacement, mDisplacement, -mDisplacement),
		glm::vec3(-mDisplacement, mDisplacement, 0.0f),
		glm::vec3(-mDisplacement, mDisplacement, mDisplacement),
		glm::vec3(-mDisplacement, mDisplacement, -mDisplacement),

		// Middle cubes
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, mDisplacement),
		glm::vec3(0.0f, 0.0f, -mDisplacement),
		glm::vec3(mDisplacement, 0.0f, 0.0f),
		glm::vec3(mDisplacement, 0.0f, mDisplacement),
		glm::vec3(mDisplacement, 0.0f, -mDisplacement),
		glm::vec3(-mDisplacement, 0.0f, 0.0f),
		glm::vec3(-mDisplacement, 0.0f, mDisplacement),
		glm::vec3(-mDisplacement, 0.0f, -mDisplacement),

		// Bottom cubes
		// Top cubes
		glm::vec3(0.0f, -mDisplacement, 0.0f),
		glm::vec3(0.0f, -mDisplacement, mDisplacement),
		glm::vec3(0.0f, -mDisplacement, -mDisplacement),
		glm::vec3(mDisplacement, -mDisplacement, 0.0f),
		glm::vec3(mDisplacement, -mDisplacement, mDisplacement),
		glm::vec3(mDisplacement, -mDisplacement, -mDisplacement),
		glm::vec3(-mDisplacement, -mDisplacement, 0.0f),
		glm::vec3(-mDisplacement, -mDisplacement, mDisplacement),
		glm::vec3(-mDisplacement, -mDisplacement, -mDisplacement),
	};

	glm::vec3 halfExtents = glm::vec3(0.5f);

	std::shared_ptr<Cube> cube1 = std::make_shared<Cube>(cubePositions[0], halfExtents, Black, Black, Black, Green, Black, Black);
	std::shared_ptr<Cube> cube2 = std::make_shared<Cube>(cubePositions[1], halfExtents, Yellow, Black, Black, Green, Black, Black);
	std::shared_ptr<Cube> cube3 = std::make_shared<Cube>(cubePositions[2], halfExtents, Black, Black, Black, Green, Black, White);
	std::shared_ptr<Cube> cube4 = std::make_shared<Cube>(cubePositions[3], halfExtents, Black, Red, Black, Green, Black, Black);
	std::shared_ptr<Cube> cube5 = std::make_shared<Cube>(cubePositions[4], halfExtents, Yellow, Red, Black, Green, Black, Black);
	std::shared_ptr<Cube> cube6 = std::make_shared<Cube>(cubePositions[5], halfExtents, Black, Red, Black, Green, Black, White);
	std::shared_ptr<Cube> cube7 = std::make_shared<Cube>(cubePositions[6], halfExtents, Black, Black, Orange, Green, Black, Black);
	std::shared_ptr<Cube> cube8 = std::make_shared<Cube>(cubePositions[7], halfExtents, Yellow, Black, Orange, Green, Black, Black);
	std::shared_ptr<Cube> cube9 = std::make_shared<Cube>(cubePositions[8], halfExtents, Black, Black, Orange, Green, Black, White);

	std::shared_ptr<Cube> cube10 = std::make_shared<Cube>(cubePositions[9], halfExtents, Black, Black, Black, Black, Black, Black);
	std::shared_ptr<Cube> cube11 = std::make_shared<Cube>(cubePositions[10], halfExtents, Yellow, Black, Black, Black, Black, Black);
	std::shared_ptr<Cube> cube12 = std::make_shared<Cube>(cubePositions[11], halfExtents, Black, Black, Black, Black, Black, White);
	std::shared_ptr<Cube> cube13 = std::make_shared<Cube>(cubePositions[12], halfExtents, Black, Red, Black, Black, Black, Black);
	std::shared_ptr<Cube> cube14 = std::make_shared<Cube>(cubePositions[13], halfExtents, Yellow, Red, Black, Black, Black, Black);
	std::shared_ptr<Cube> cube15 = std::make_shared<Cube>(cubePositions[14], halfExtents, Black, Red, Black, Black, Black, White);
	std::shared_ptr<Cube> cube16 = std::make_shared<Cube>(cubePositions[15], halfExtents, Black, Black, Orange, Black, Black, Black);
	std::shared_ptr<Cube> cube17 = std::make_shared<Cube>(cubePositions[16], halfExtents, Yellow, Black, Orange, Black, Black, Black);
	std::shared_ptr<Cube> cube18 = std::make_shared<Cube>(cubePositions[17], halfExtents, Black, Black, Orange, Black, Black, White);

	std::shared_ptr<Cube> cube19 = std::make_shared<Cube>(cubePositions[18], halfExtents, Black, Black, Black, Black, Blue, Black);
	std::shared_ptr<Cube> cube20 = std::make_shared<Cube>(cubePositions[19], halfExtents, Yellow, Black, Black, Black, Blue, Black);
	std::shared_ptr<Cube> cube21 = std::make_shared<Cube>(cubePositions[20], halfExtents, Black, Black, Black, Black, Blue, White);
	std::shared_ptr<Cube> cube22 = std::make_shared<Cube>(cubePositions[21], halfExtents, Black, Red, Black, Black, Blue, Black);
	std::shared_ptr<Cube> cube23 = std::make_shared<Cube>(cubePositions[22], halfExtents, Yellow, Red, Black, Black, Blue, Black);
	std::shared_ptr<Cube> cube24 = std::make_shared<Cube>(cubePositions[23], halfExtents, Black, Red, Black, Black, Blue, White);
	std::shared_ptr<Cube> cube25 = std::make_shared<Cube>(cubePositions[24], halfExtents, Black, Black, Orange, Black, Blue, Black);
	std::shared_ptr<Cube> cube26 = std::make_shared<Cube>(cubePositions[25], halfExtents, Yellow, Black, Orange, Black, Blue, Black);
	std::shared_ptr<Cube> cube27 = std::make_shared<Cube>(cubePositions[26], halfExtents, Black, Black, Orange, Black, Blue, White);

	// Disable collision for center-most cube
	// cube10->disableCollision();

	mCubeMap.insert(std::make_pair(cube1->getID(), cube1));
	mCubeMap.insert(std::make_pair(cube2->getID(), cube2));
	mCubeMap.insert(std::make_pair(cube3->getID(), cube3));
	mCubeMap.insert(std::make_pair(cube4->getID(), cube4));
	mCubeMap.insert(std::make_pair(cube5->getID(), cube5));
	mCubeMap.insert(std::make_pair(cube6->getID(), cube6));
	mCubeMap.insert(std::make_pair(cube7->getID(), cube7));
	mCubeMap.insert(std::make_pair(cube8->getID(), cube8));
	mCubeMap.insert(std::make_pair(cube9->getID(), cube9));

	mCubeMap.insert(std::make_pair(cube10->getID(), cube10));
	mCubeMap.insert(std::make_pair(cube11->getID(), cube11));
	mCubeMap.insert(std::make_pair(cube12->getID(), cube12));
	mCubeMap.insert(std::make_pair(cube13->getID(), cube13));
	mCubeMap.insert(std::make_pair(cube14->getID(), cube14));
	mCubeMap.insert(std::make_pair(cube15->getID(), cube15));
	mCubeMap.insert(std::make_pair(cube16->getID(), cube16));
	mCubeMap.insert(std::make_pair(cube17->getID(), cube17));
	mCubeMap.insert(std::make_pair(cube18->getID(), cube18));

	mCubeMap.insert(std::make_pair(cube19->getID(), cube19));
	mCubeMap.insert(std::make_pair(cube20->getID(), cube20));
	mCubeMap.insert(std::make_pair(cube21->getID(), cube21));
	mCubeMap.insert(std::make_pair(cube22->getID(), cube22));
	mCubeMap.insert(std::make_pair(cube23->getID(), cube23));
	mCubeMap.insert(std::make_pair(cube24->getID(), cube24));
	mCubeMap.insert(std::make_pair(cube25->getID(), cube25));
	mCubeMap.insert(std::make_pair(cube26->getID(), cube26));
	mCubeMap.insert(std::make_pair(cube27->getID(), cube27));
}

void RubiksCube::selectCube(int selectedID)
{
	mSelectedCubeID = selectedID;
	mLastSelectedCubeID = -1;
	mbPlayerRotating = true;
	mCubeMap.at(selectedID)->setHighlight(true);
}

void RubiksCube::selectSection()
{
	// Return early if no axis selected
	if (mSelectedAxis < 0) return;

	findSectionCubes();
	highlightSection();

	for (int cubeID : mSelectedSectionIDs)
	{
		mCubeMap.at(cubeID)->initiateTurn();
	}
}

void RubiksCube::findSectionCubes()
{
	std::shared_ptr<Cube> selectedCube = mCubeMap.at(mSelectedCubeID);
	float axisValue = selectedCube->getCurrentPosition()[mSelectedAxis];

	mSelectedSectionIDs = std::vector<int>();

	CubeMap::const_iterator ent;

	for (ent = mCubeMap.begin(); ent != mCubeMap.end(); ent++)
	{
		if (ent->first == selectedCube->getID())
			continue;

		if (ent->second->getCurrentPosition()[mSelectedAxis] == axisValue)
			mSelectedSectionIDs.push_back(ent->first);
	}

	mSelectedSectionIDs.push_back(selectedCube->getID());
}

void RubiksCube::findSectionCubes(RubiksSection section, int axis)
{
	float axisValue = 0.0f;
	switch (mScrambleSection)
	{
	case RubiksSection::BACK:
		axisValue = -1.0f;
		break;
	case RubiksSection::FRONT:
		axisValue = 1.0f;
		break;
	}

	axisValue *= mDisplacement;
	mSelectedSectionIDs = std::vector<int>();

	CubeMap::const_iterator ent;

	for (ent = mCubeMap.begin(); ent != mCubeMap.end(); ent++)
	{
		if (ent->second->getCurrentPosition()[axis] == axisValue)
			mSelectedSectionIDs.push_back(ent->first);
	}
}

void RubiksCube::findSelectedFaceNormal(glm::vec3 collisionPoint)
{
	if (mSelectedCubeID < 0) return;

	glm::vec3 cubePosition = mCubeMap[mSelectedCubeID]->getCurrentPosition();
	glm::vec3 cubeExtents = mCubeMap[mSelectedCubeID]->getHalfExtents();

	float threshold = 0.00015f;

	for (int i = 0; i < 3; i++)
	{
		bool pFaceCollision = collisionPoint[i] >= cubePosition[i] + cubeExtents[i] - threshold && collisionPoint[i] <= cubePosition[i] + cubeExtents[i] + threshold;
		bool nFaceCollision = collisionPoint[i] <= cubePosition[i] - cubeExtents[i] + threshold && collisionPoint[i] >= cubePosition[i] - cubeExtents[i] - threshold;

		if (pFaceCollision || nFaceCollision)
		{
			mSelectedFaceNormal = i;
			break;
		}
	}

	std::cout << "Selected Face Normal: " << mSelectedFaceNormal << std::endl;
}

void RubiksCube::calculateCameraDirection(float cameraDirection)
{
	float restrainedYaw = cameraDirection;

	if (restrainedYaw > 360.0f)
	{
		while (restrainedYaw > 360.0f)
		{
			restrainedYaw -= 360.0f;
		}
	}
	else if (restrainedYaw < 0.0f)
	{
		while (restrainedYaw < 0.0f)
		{
			restrainedYaw += 360.0f;
		}
	}

	mCameraDirection = restrainedYaw;
}

void RubiksCube::highlightSection()
{
	for (int cubeID : mSelectedSectionIDs)
	{
		if (cubeID == mSelectedCubeID)
			continue;

		mCubeMap.at(cubeID)->setHighlight(true);
	}
}

void RubiksCube::clearSectionCubes()
{
	for (int cubeID : mSelectedSectionIDs)
	{
		mCubeMap.at(cubeID)->setHighlight(false);
	}

	mSelectedSectionIDs.clear();
}

void RubiksCube::changeSectionCubes()
{
	toPreviousStateImmediate();

	// Clear all section cubes except the current selected cube
	for (int cubeID : mSelectedSectionIDs)
	{
		if (cubeID == mSelectedCubeID)
			continue;

		mCubeMap.at(cubeID)->setHighlight(false);
	}

	mSelectedSectionIDs.clear();
	selectSection();

}

void RubiksCube::rotateSectionPercentage(float targetRadians, float dt, int axis)
{
	for (int cubeID : mSelectedSectionIDs)
	{
		switch (axis)
		{
		case 0:
			mCubeMap.at(cubeID)->rotateXByPercentage(targetRadians, dt);
			break;
		case 1:
			mCubeMap.at(cubeID)->rotateYByPercentage(targetRadians, dt);
			break;
		case 2:
			mCubeMap.at(cubeID)->rotateZByPercentage(targetRadians, dt);
			break;
		}
	}
}

void RubiksCube::rotateSectionImmediate(float radians, int axis)
{
	for (int cubeID : mSelectedSectionIDs)
	{
		switch (axis)
		{
		case 0:
			mCubeMap.at(cubeID)->rotateXImmediate(radians);
			break;
		case 1:
			mCubeMap.at(cubeID)->rotateYImmediate(radians);
			break;
		case 2:
			mCubeMap.at(cubeID)->rotateZImmediate(radians);
			break;
		}
	}

	clampRotatingCubes();
	updateSectionColliders();
}

void RubiksCube::toPreviousStateImmediate()
{
	for (int cubeID : mSelectedSectionIDs)
		mCubeMap.at(cubeID)->toPreviousStateImmediate();
	mPlayerRotationInput = 0.0f;
}

void RubiksCube::toPreviousState()
{
	if (mPlayerRotationInput < FLT_EPSILON && mPlayerRotationInput > -FLT_EPSILON)
	{
		toPreviousStateImmediate();
		clearSectionCubes();
		return;
	}

	//mbAutoResetRotation = true;
	//mAutoFromDegrees = mPlayerRotationInput > 0.0f ? -90.0f : 90.0f;
	//mCurrentRotateTime = glm::abs(mPlayerRotationInput) / 90.0f * mAutoRotateTime;
	mPlayerRotationInput = 0.0f;
}

void RubiksCube::updateSectionColliders()
{
	for (int cubeID : mSelectedSectionIDs)
	{
		mCubeMap.at(cubeID)->updateColliderPosition();
	}
}

// TODO: Remove and move logic into Cube?
void RubiksCube::clampRotatingCubes()
{
	for (int cubeID : mSelectedSectionIDs)
	{
		glm::vec3 currentPosition = mCubeMap.at(cubeID)->getCurrentPosition();
		mCubeMap.at(cubeID)->setCurrentPosition(clampPosition(currentPosition));
	}
}

float RubiksCube::clampCoordinate(float coordinate) const
{
	if (coordinate > -mErrorMargin && coordinate < mErrorMargin)
		return 0.0f;
	if (coordinate > mErrorMargin && coordinate < mDisplacement || coordinate > mDisplacement)
		return mDisplacement;
	if (coordinate < -mErrorMargin && coordinate > -mDisplacement || coordinate < -mDisplacement)
		return -mDisplacement;
	return coordinate;
}

glm::vec3 RubiksCube::clampPosition(const glm::vec3& position) const
{
	glm::vec3 newPos = glm::vec3(clampCoordinate(position.x), clampCoordinate(position.y), clampCoordinate(position.z));
	return newPos;
}


// Perform all scramble logic including setting up
void RubiksCube::executeScrambleSmooth(float deltaTime)
{
	if (!mbIsScrambling) return;

	if (mbIsRotating)
	{
		performSmoothScrambleRotation(deltaTime);
	}
	else
	{
		if (mCurrentScrambleCount < mTargetScrambleCount)
		{
			setupScrambleRotation();
			performSmoothScrambleRotation(deltaTime);
		}
		else
		{
			mbIsScrambling = false;
			onScrambleComplete();
		}
	}
}

void RubiksCube::setupScrambleRotation()
{
	// TODO: Try Using PCG32
	srand(time(0));
	int newAxis = rand() % 3;
	int newSection = (rand() % 3) + 1;
	int oldSection = (int)mScrambleSection;

	if (newAxis == mScrambleAxis && newSection == oldSection)
	{
		if (newAxis == newSection)
		{
			newAxis = (newAxis + 3) % 3;
			newSection = ((newSection + 2) % 3) + 1;
		}
		else if (newAxis % 2 > 0 && newSection % 2 > 0)
		{
			newAxis = (newAxis + 2) % 3;
			newSection = ((newSection + 4) % 3) + 1;
		}
		else if (newAxis % 2 == 0 && newSection % 2 == 0)
		{
			newAxis = (newAxis + 1) % 3;
			newSection = ((newSection + 2) % 3) + 1;
		}
		else
		{
			int temp = newAxis;
			newAxis = newSection % 3;
			newSection = temp + 1;
		}
	}

	mScrambleAxis = newAxis;
	mScrambleSection = static_cast<RubiksSection>(newSection);
	mCurrentRotateTime = 0.0f;
	mbIsRotating = true;
	findSectionCubes(mScrambleSection, mScrambleAxis);
}

// TODO: FIGURE OUT HOW TO IMPLEMENT THIS!!!!
void RubiksCube::processCubeSelection(const Ray& ray, float cameraDirection)
{
	// Ensure not scrambling or already being rotated
	// Auto Complete Rotation????
	if (mbPlayerRotating || mbIsScrambling) return;

	// If rotation is resetting, select the previously selected cube

	int selectedCube = -1;
	glm::vec3 end = ray.Origin + ray.Direction * ray.Length;
	glm::vec3 collisionPoint;

	if (CollisionSystem::Instance()->intersectRayAllAABB(ray.Origin, end, selectedCube, collisionPoint))
	{
		selectCube(selectedCube);
		findSelectedFaceNormal(collisionPoint);

		// If rotating face, select section right away

		calculateCameraDirection(cameraDirection);
	}
	else
	{
		std::cout << "No cube selected :(" << std::endl;
	}

}

void RubiksCube::processCubeReleased()
{
	if (!mbPlayerRotating) return;
	if (mSelectedSectionIDs.size() <= 0)
	{
		mCubeMap[mSelectedCubeID]->setHighlight(false);
		mLastSelectedCubeID = -1;
		mSelectedCubeID = -1;
		mbPlayerRotating = false;
		return;
	}

	// releaseCube();
	toPreviousState();
}

void RubiksCube::performSmoothScrambleRotation(float deltaTime)
{
	mCurrentRotateTime += deltaTime;
	float dt = mCurrentRotateTime / mRotateCompletionTime;
	
	if (dt >= 1.0f)
	{
		performImmediateScrambleRotation();
		mCurrentScrambleCount += 1;
		mbIsRotating = false;
		return;
	}

	rotateSectionPercentage(glm::radians(mScrambleTargetRotation), dt, mScrambleAxis);
}

void RubiksCube::performImmediateScrambleRotation()
{
	rotateSectionImmediate(glm::radians(mScrambleTargetRotation), mScrambleAxis);
	clearSectionCubes();
}
