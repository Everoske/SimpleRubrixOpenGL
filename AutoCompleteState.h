#ifndef AUTO_COMPLETE_STATE_H
#define AUTO_COMPLETE_STATE_H

#include "RubikState.h"

class AutoCompleteState : public RubikState
{
public:
	AutoCompleteState(float autoCompleteTime) : mTargetRotationTime{autoCompleteTime} {}

	virtual void Enter(RubiksCube* cube);
	virtual void Execute(RubiksCube* cube, float deltaTime);
	virtual void Exit(RubiksCube* cube);

private:
	float mTargetRotationTime;
	
	float mTargetDegrees = 0.0f;
	float mRotationTimer = 0.0f;

};

#endif