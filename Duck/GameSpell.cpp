#include "GameSpell.h"
#include "GameData.h"

void GameSpell::ReadFromBaseAddress(int addr)
{
	lvl = ReadInt(addr + Offset::SpellSlotLevel);
	readyAt = ReadFloat(addr + Offset::SpellSlotTime);
	value = ReadFloat(addr + Offset::SpellSlotValue);

	int spellInfo = ReadInt(addr + Offset::SpellSlotSpellInfo);
	int spellData = ReadInt(spellInfo + Offset::SpellInfoSpellData);

	name = Memory::ReadString(ReadInt(spellData + Offset::SpellDataSpellName));
	staticData = GameData::GetSpell(name);
}

void GameSpell::ImGuiDraw()
{
	ImGui::Text("Name: %s", name.c_str());
	ImGui::DragInt("Level", &lvl);
	ImGui::DragFloat("Ready At", &readyAt);
	ImGui::DragFloat("Value", &value);
}