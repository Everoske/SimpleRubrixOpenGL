#include "RubikStateMachine.h"

RubikStateMachine::RubikStateMachine(RubiksCube* owner) :
	mOwner{ owner },
	mCurrentState{nullptr},
	mPreviousState{nullptr}
{
	mScrambleState = new ScrambleState(1.5f, 5);
}

RubikStateMachine::~RubikStateMachine()
{
	mCurrentState = nullptr;
	mPreviousState = nullptr;
	delete mScrambleState;
}

void RubikStateMachine::changeState(RubikState* newState)
{
	assert(newState && "RubikStateMachine::changeState: Trying to change to a null state");

	mPreviousState = mCurrentState;
	mCurrentState->Exit(mOwner);
	mCurrentState = newState;
	mCurrentState->Enter(mOwner);
}

bool RubikStateMachine::isInState(const RubikState& state) const
{
	return typeid(*mCurrentState) == typeid(state);
}

void RubikStateMachine::update(float deltaTime) const
{
	if (mCurrentState)
		mCurrentState->Execute(mOwner, deltaTime);
}