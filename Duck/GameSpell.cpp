#include "GameSpell.h"
#include "GameData.h"
#include "Strings.h"
#include "Duck.h"

void GameSpell::ReadFromBaseAddress(int addr)
{
	lvl = ReadInt(addr + Offset::SpellSlotLevel);
	readyAt = ReadFloat(addr + Offset::SpellSlotTime);
	value = ReadFloat(addr + Offset::SpellSlotValue);

	int spellInfo = ReadInt(addr + Offset::SpellSlotSpellInfo);
	int spellData = ReadInt(spellInfo + Offset::SpellInfoSpellData);

	name = Memory::ReadString(ReadInt(spellData + Offset::SpellDataSpellName));
	name = Strings::ToLower(name);
	staticData = GameData::GetSpell(name);
}

void GameSpell::ImGuiDraw()
{
	ImGui::Text("Name: %s", name.c_str());
	ImGui::DragInt("Level", &lvl);
	ImGui::DragFloat("Ready At", &readyAt);
	ImGui::DragFloat("Value", &value);
}

float GameSpell::GetRemainingCooldown()
{
	float cd = readyAt - Duck::CurrentGameState->time;
	return (cd >= 0.f ? cd : 0.0f);
}

object GameSpell::GetStaticData()
{
	return object(ptr(staticData));
}