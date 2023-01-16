#include "GameUnit.h"
#include "GameData.h"

GameUnit::GameUnit()
{

}

GameUnit::GameUnit(std::string name)
	:GameObject(name)
{
	staticData = GameData::GetUnit(name);
}

void GameUnit::ReadFromBaseAddress(int addr)
{
	GameObject::ReadFromBaseAddress(addr);
	mana = ReadFloat(addr + Offset::ObjMana);
	health = ReadFloat(addr + Offset::ObjHealth);
	maxHealth = ReadFloat(addr + Offset::ObjMaxHealth);
	armor = ReadFloat(addr + Offset::ObjArmor);
	magicRes = ReadFloat(addr + Offset::ObjMagicRes);
	baseAtk = ReadFloat(addr + Offset::ObjBaseAtk);
	bonusAtk = ReadFloat(addr + Offset::ObjBonusAtk);
	moveSpeed = ReadFloat(addr + Offset::ObjMoveSpeed);
	expiry = ReadFloat(addr + Offset::ObjExpiry);
	crit = ReadFloat(addr + Offset::ObjCrit);
	critMulti = ReadFloat(addr + Offset::ObjCritMulti);
	abilityPower = ReadFloat(addr + Offset::ObjAbilityPower);
	atkSpeedMulti = ReadFloat(addr + Offset::ObjAtkSpeedMulti);
	attackRange = ReadFloat(addr + Offset::ObjAtkRange);

	isDead = ReadInt(addr + Offset::ObjSpawnCount) % 2;
	lvl = ReadInt(addr + Offset::ObjLvl);
}

void GameUnit::ImGuiDraw()
{
	GameObject::ImGuiDraw();
	ImGui::Separator();

	ImGui::DragFloat("Mana", &mana);
	ImGui::DragFloat("Health", &health);
	ImGui::DragFloat("MaxHealth", &maxHealth);
	ImGui::DragFloat("Armor", &armor);
	ImGui::DragFloat("MagicRes", &magicRes);
	ImGui::DragFloat("BaseAtk", &baseAtk);
	ImGui::DragFloat("BonusAtk", &bonusAtk);
	ImGui::DragFloat("MoveSpeed", &moveSpeed);
	ImGui::DragFloat("Expiry", &expiry);
	ImGui::DragFloat("Crit", &crit);
	ImGui::DragFloat("CritMulti", &critMulti);
	ImGui::DragFloat("AbilityPower", &abilityPower);
	ImGui::DragFloat("AtkSpeedMulti", &atkSpeedMulti);
	ImGui::DragFloat("AttackRange", &attackRange);

	ImGui::Checkbox("IsDead", &isDead);
	ImGui::DragInt("Level", &lvl);

	ImGui::Separator();
	if (ImGui::TreeNode("Static Data")) {
		
		staticData->ImGuiDraw();
		ImGui::TreePop();
	}
}