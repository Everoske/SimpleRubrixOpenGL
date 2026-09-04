#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "RubikState.h"

class PlayState : public RubikState
{
public:
	PlayState() {}

	virtual void Enter(RubiksCube* cube);
	virtual void Execute(RubiksCube* cube, float deltaTime);
	virtual void Exit(RubiksCube* cube);
};


#endif