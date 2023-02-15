#pragma once
#include "GameObject.h"
#include "UnitInfo.h"
#include "SpellCast.h"

class GameUnit : public GameObject {

public:



	bool  isDead;
	float mana;
	float health;
	float maxHealth;
	float armor;
	float magicRes;
	float baseAtk;
	float bonusAtk;
	float moveSpeed;
	int   lvl;
	float expiry;
	float crit;
	float critMulti;
	float abilityPower;
	float atkSpeedMulti;
	float attackRange;
	bool  hasCastingSpell = false;

	UnitInfo* staticData;
	SpellCast castingSpell;

	GameUnit();
	GameUnit(std::string name);

	void ReadFromBaseAddress(int addr);
	void ImGuiDraw();
	bool HasTags(UnitTag tag);
	float GetAttackSpeed();
	object GetStaticData();
	object GetCastingSpell();
};
