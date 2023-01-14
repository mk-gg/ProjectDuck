#pragma once
#include "GameState.h"

class GameReader 
{

private:
	GameState state;
	
public:
	const GameState* GetNextState();

};