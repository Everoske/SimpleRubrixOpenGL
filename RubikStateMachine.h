#ifndef RUBIK_STATE_MACHINE_H
#define RUBIK_STATE_MACHINE_H

#include <memory>
#include "RubikState.h"
#include "ScrambleState.h"

class RubikStateMachine
{
private:
	RubiksCube* mOwner;
	std::shared_ptr<RubikState> mCurrentState;
	std::shared_ptr<RubikState> mPreviousState;

	std::shared_ptr<ScrambleState> mScrambleState;

public:
	RubikStateMachine(RubiksCube* owner);

	void setCurrentState(std::shared_ptr<RubikState> s) { mCurrentState = s; }
	void setPreviousState(std::shared_ptr<RubikState> s) { mPreviousState = s; }
	void changeState(std::shared_ptr<RubikState> newState);

	bool isInState(const RubikState& st) const;

	void update(float deltaTime) const;

	std::shared_ptr<ScrambleState> getScrambleState() const { return mScrambleState; }
};


#endif