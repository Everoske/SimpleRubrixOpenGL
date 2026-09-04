#include "PlayState.h"
#include <iostream>

void PlayState::Enter(RubiksCube* cube)
{
	std::cout << "Entering Play State" << std::endl;
}

void PlayState::Execute(RubiksCube* cube, float deltaTime)
{
	// Just check if the player is selecting the cube
	// If player selecting a cube, change to PlayerRotateState


}

void PlayState::Exit(RubiksCube* cube)
{

}