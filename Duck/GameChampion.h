#pragma once
#include "GameUnit.h"
#include "GameSpell.h"
#include "ItemInfo.h"

using namespace boost::python;



class GameChampion : public GameUnit 
{

public:
	GameChampion();
	GameChampion(std::string name);

	void ReadFromBaseAddress(int addr);
	void ImGuiDraw();

	
	object SpellsToPy();
	object ItemsToPy();

	Vector2 GetHpBarPosition();
	GameSpell spells[6];
	ItemInfo* items[6];


};


