#ifndef ROTATE_SLICE_STATE_H
#define ROTATE_SLICE_STATE_H

#include "RubikState.h"

class RotateSliceState : RubikState
{
public:
	RotateSliceState() {}

	virtual void Enter(RubiksCube* cube);
	virtual void Execute(RubiksCube* cube, float deltaTime);
	virtual void Exit(RubiksCube* cube);

private:
	void calculateAxisFromMovement(float deltaTime);
};

#endif