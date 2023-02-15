#pragma once
#include "GameHud.h"
#include "GameRenderer.h"
#include "GameObject.h"
#include "GameChampion.h"
#include "GameMissile.h"
#include "GameUnit.h"
#include "GameMinion.h"
#include "GameJungle.h"
#include "GameTurret.h"

#include <vector>
#include <map>
struct GameState 
{
	GameRenderer renderer;
	GameHud      hud;
	float time;
	bool gameStarted;
	float ping;
	
	std::vector<std::shared_ptr<GameMinion>>    minions;
	std::vector<std::shared_ptr<GameJungle>>    jungle;
	std::vector<std::shared_ptr<GameTurret>>    turrets;
	std::vector<std::shared_ptr<GameChampion>>  champions;
	std::vector<std::shared_ptr<GameMissile>>   missiles;
	std::vector<std::shared_ptr<GameUnit>>      others;
	std::map<int, std::shared_ptr<GameObject>> objectCache;
	std::shared_ptr<GameChampion> player;
	std::shared_ptr<GameObject> hovered;


};