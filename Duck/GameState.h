#pragma once
#include "GameHud.h"
#include "GameRenderer.h"
#include "GameObject.h"
#include <map>
struct GameState 
{
	GameRenderer renderer;
	GameHud      hud;
	float time;
	std::map<int, std::shared_ptr<GameObject>> objectCache;
	std::shared_ptr<GameObject> player;
};