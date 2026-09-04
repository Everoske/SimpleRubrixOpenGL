#ifndef SCRAMBLE_STATE_H
#define SCRAMLBE_STATE_H

#include "RubikState.h"

class ScrambleState : public RubikState
{
public:
	ScrambleState(float scrambleRotationTime, int scrambleCount) : 
		mTargetRotationTime{scrambleRotationTime}, mTargetScrambleCount{scrambleCount} {}

	virtual void Enter(RubiksCube* cube);
	virtual void Execute(RubiksCube* cube, float deltaTime);
	virtual void Exit(RubiksCube* cube);

private:
	void setupScrambleRotation(RubiksCube* cube);
	void performSmoothScrambleRotation(RubiksCube* cube, float deltaTime);
	void performImmediateScrambleRotation(RubiksCube* cube);

	float mTargetRotationTime;
	int mTargetScrambleCount;

	float mRotationTimer = 0.0f;
	int mCurrentScrambleCount = 0;
	int mScrambleAxis = -1;
	RubiksSection mScrambleSection = RubiksSection::NONE;
};

#endif