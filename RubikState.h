#ifndef RUBIK_STATE_H
#define RUBIK_STATE_H

#include "RubiksCube.h"

class RubikState
{
	virtual void Enter(RubiksCube&) = 0;
	virtual void Execute(RubiksCube&, float) = 0;
	virtual void Exit(RubiksCube&) = 0;
};

#endif