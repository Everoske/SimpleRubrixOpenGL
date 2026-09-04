#ifndef RUBIK_STATE_MACHINE_H
#define RUBIK_STATE_MACHINE_H

#include "RubikState.h"
#include "ScrambleState.h"

class RubikStateMachine
{
private:
	RubiksCube* mOwner;
	RubikState* mCurrentState;
	RubikState* mPreviousState;

	ScrambleState* mScrambleState;

public:
	RubikStateMachine(RubiksCube* owner);
	~RubikStateMachine();

	void setCurrentState(RubikState* state) { mCurrentState = state; }
	void setPreviousState(RubikState* state) { mPreviousState = state; }
	void changeState(RubikState* newState);

	bool isInState(const RubikState& state) const;

	void update(float deltaTime) const;

	ScrambleState* getScrambleState() const { return mScrambleState; }
};


#endif