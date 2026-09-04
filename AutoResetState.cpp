#include "AutoResetState.h"

void AutoResetState::Enter(RubiksCube* cube)
{
	//mFromDegrees = cube.getPlayerRotationInput() > 0.0f ? -90.0f : 90.0f;
	//mRotationTimer = glm::abs(cube.getPlayerRotationInput()) / 90.0f * mTargetRotationTime;
	//cube.setPlayerRotationInput(0.0f);
}

void AutoResetState::Execute(RubiksCube* cube, float deltaTime)
{
	mRotationTimer -= deltaTime;
	float dt = mRotationTimer / mTargetRotationTime;

	if (dt <= 0.0f)
	{
		//cube.rotateSectionImmediate(glm::radians(mFromDegrees));
		cube->teardownRotation();
		// cube.changeState(cube.playState);
		return;
	}

	//cube.rotateSectionPercentage(glm::radians(mFromDegrees), dt);
}

void AutoResetState::Exit(RubiksCube* cube)
{
	// Handle Exit When Reset is Incomplete
	// Recalculate and set player rotation amount
	// float calculatedPlayerInput = calculatePlayerInput();
	// cube.setPlayerRotationInput(calculatedPlayerInput);

	// Handle Exit When Reset is Complete
}