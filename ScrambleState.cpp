#include "ScrambleState.h"
#include <ctime>

void ScrambleState::Enter(RubiksCube& cube)
{
	mCurrentScrambleCount = 0;
}

void ScrambleState::Execute(RubiksCube& cube, float deltaTime)
{
	if (cube.isRotationInProgress())
	{
		performSmoothScrambleRotation(cube, deltaTime);
	}
	else
	{
		if (mCurrentScrambleCount < mTargetScrambleCount)
		{
			setupScrambleRotation(cube);
			performSmoothScrambleRotation(cube, deltaTime);
		}
		else
		{
			// cube.changeState(cube.playState);
		}
	}
}

void ScrambleState::Exit(RubiksCube& cube)
{
	// Any additional cleanup goes here
}

void ScrambleState::setupScrambleRotation(RubiksCube& cube)
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
	mRotationTimer = 0.0f;
	cube.initiateRotation(mScrambleAxis, mScrambleSection);
}

void ScrambleState::performSmoothScrambleRotation(RubiksCube& cube, float deltaTime)
{
	mRotationTimer += deltaTime;
	float dt = mRotationTimer / mTargetRotationTime;

	if (dt >= 1.0f)
	{
		performImmediateScrambleRotation(cube);
		mCurrentScrambleCount += 1;
		return;
	}

	cube.rotateSectionPercentage(glm::radians(90.0f), dt, mScrambleAxis);
}

void ScrambleState::performImmediateScrambleRotation(RubiksCube& cube)
{
	cube.rotateSectionImmediate(glm::radians(90.0f), mScrambleAxis);
	cube.teardownRotation();
}