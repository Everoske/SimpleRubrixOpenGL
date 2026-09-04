#ifndef AUTO_RESET_STATE_H
#define AUTO_RESET_STATE_H

#include "RubikState.h"

class AutoResetState : public RubikState
{
public:
	AutoResetState(float autoCompleteTime) : mTargetRotationTime{autoCompleteTime} {}

	virtual void Enter(RubiksCube* cube);
	virtual void Execute(RubiksCube* cube, float deltaTime);
	virtual void Exit(RubiksCube* cube);

private:
	float mTargetRotationTime;

	float mFromDegrees = 0.0f;
	float mRotationTimer = 0.0f;
};


#endif