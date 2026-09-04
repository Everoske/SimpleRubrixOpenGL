#ifndef RUBIK_STATE_MACHINE_H
#define RUBIK_STATE_MACHINE_H

#include "RubikState.h"

class ScrambleState;
class PlayState;

class RubikStateMachine
{
private:
	RubiksCube* mOwner;
	RubikState* mCurrentState;
	RubikState* mPreviousState;

	ScrambleState* mScrambleState;
	PlayState* mPlayState;

public:
	RubikStateMachine(RubiksCube* owner);
	~RubikStateMachine();

	void setCurrentState(RubikState* state) { mCurrentState = state; }
	void setPreviousState(RubikState* state) { mPreviousState = state; }
	void changeState(RubikState* newState);

	void enterScrambleState();
	void changeToPlayState();

	bool isInState(const RubikState& state) const;

	void update(float deltaTime) const;
};


#endif