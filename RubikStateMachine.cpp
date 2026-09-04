#include "RubikStateMachine.h"

RubikStateMachine::RubikStateMachine(RubiksCube* owner) :
	mOwner{ owner },
	mCurrentState{nullptr},
	mPreviousState{nullptr}
{
	mScrambleState = std::make_shared<ScrambleState>();
}

void RubikStateMachine::changeState(std::shared_ptr<RubikState> newState)
{
	assert(newState && "RubikStateMachine::changeState: Trying to change to a null state");

	mPreviousState = mCurrentState;
	mCurrentState->Exit(mOwner);
	mCurrentState = newState;
	mCurrentState->Enter(mOwner);
}

bool RubikStateMachine::isInState(const RubikState& st) const
{
	return typeid(*mCurrentState) == typeid(st);
}

void RubikStateMachine::update(float deltaTime) const
{
	if (mCurrentState)
		mCurrentState->Execute(mOwner, deltaTime);
}