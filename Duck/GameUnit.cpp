#include "GameUnit.h"
#include "GameData.h"

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
	attackRange = ReadFloat(addr + 0x12B8);

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
		ImGui::DragFloat("Acquisition Radius", &staticData->acquisitionRange);
		ImGui::DragFloat("Gameplay Radius", &staticData->gameplayRadius);
		ImGui::DragFloat("Pathing Radius", &staticData->pathRadius);
		ImGui::DragFloat("Selection Radius", &staticData->selectionRadius);
		ImGui::DragFloat("Base Attack Range", &staticData->baseAttackRange);
		ImGui::DragFloat("Base Movement Speed", &staticData->baseMovementSpeed);
		ImGui::DragFloat("Basic Attack Speed", &staticData->basicAttackMissileSpeed);
		ImGui::DragFloat("Basic Attack Windup", &staticData->basicAttackWindup);
		ImGui::DragFloat("Attack Speed Ratio", &staticData->attackSpeedRatio);
		ImGui::DragFloat("HP Bar Height", &staticData->healthBarHeight);
		ImGui::Text("Tags");
		ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.f), staticData->StringifyTags().c_str());

		ImGui::TreePop();
	}
}