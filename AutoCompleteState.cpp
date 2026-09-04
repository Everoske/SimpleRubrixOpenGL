#include "AutoCompleteState.h"

void AutoCompleteState::Enter(RubiksCube* cube)
{
	// if (cube.getPlayerInput() > FLT_EPSILON)
	//     mTargetDegrees = 90.0f;
	// else
	//     mTargetDegrees = -90.0f;

	mRotationTimer = 0.0f;
}

void AutoCompleteState::Execute(RubiksCube* cube, float deltaTime)
{
	mRotationTimer += deltaTime;
	float dt = mRotationTimer / mTargetRotationTime;

	if (dt >= 1.0f)
	{
		//cube.rotateSectionImmediate(glm::radians(mTargetDegrees));
		cube->teardownRotation();
		// cube.changeState(cube.playState);
		return;
	}

	//cube.rotateSectionPercentage(glm::radians(mTargetDegrees), dt);
}

void AutoCompleteState::Exit(RubiksCube* cube)
{
	// Should only exit once it completes rotation
}