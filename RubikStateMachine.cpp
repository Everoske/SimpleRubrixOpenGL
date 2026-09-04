#include "RubikStateMachine.h"
#include "ScrambleState.h"
#include "PlayState.h"

RubikStateMachine::RubikStateMachine(RubiksCube* owner) :
	mOwner{ owner },
	mCurrentState{nullptr},
	mPreviousState{nullptr}
{
	mScrambleState = new ScrambleState(0.5f, 5);
	mPlayState = new PlayState();
}

RubikStateMachine::~RubikStateMachine()
{
	mCurrentState = nullptr;
	mPreviousState = nullptr;
	delete mScrambleState;
	delete mPlayState;
}

void RubikStateMachine::changeState(RubikState* newState)
{
	assert(newState && "RubikStateMachine::changeState: Trying to change to a null state");

	mPreviousState = mCurrentState;
	mCurrentState->Exit(mOwner);
	mCurrentState = newState;
	mCurrentState->Enter(mOwner);
}

void RubikStateMachine::enterScrambleState()
{
	setCurrentState(mScrambleState);
}

void RubikStateMachine::changeToPlayState()
{
	changeState(mPlayState);
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