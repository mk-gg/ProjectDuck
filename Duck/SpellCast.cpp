#include "SpellCast.h"
#include "Offset.h"
#include "Memory.h"
#include "GameData.h"

void SpellCast::ReadFromBaseAddress(int address)
{
	int spellInfo = ReadInt(address + Offset::SpellCastSpellInfo);
	int spellData = ReadInt(spellInfo + Offset::SpellInfoSpellData);
	int nameAddr = ReadInt(spellData + Offset::SpellDataSpellName);
	name = Memory::ReadString(nameAddr);
	name = Strings::ToLower(name);
	staticData = GameData::GetSpell(name);

	memcpy(&start, AsPtr(address + Offset::SpellCastStart), sizeof(Vector3));
	memcpy(&end, AsPtr(address + Offset::SpellCastEnd), sizeof(Vector3));

	srcIndex = ReadShort(address + Offset::SpellCastSrcIdx);
	destIndex = ReadShort(address + Offset::SpellCastDestIdx);
	timeBegin = ReadFloat(address + Offset::SpellCastStartTime);
	castTime = ReadFloat(address + Offset::SpellCastCastTime);
}

void SpellCast::ImGuiDraw()
{
	ImGui::Text(name.c_str());
	start.ImGuiDraw("Start Position");
	end.ImGuiDraw("End Position");

	int srcIdx = srcIndex;
	int destIdx = destIndex;
	ImGui::DragInt("Src Index", &srcIdx);
	ImGui::DragInt("Dest Index", &destIdx);
	ImGui::DragFloat("Time Begin", &timeBegin);
	ImGui::DragFloat("Cast Time", &castTime);

	ImGui::Separator();
	if (staticData != nullptr)
		staticData->ImGuiDraw();
}

object SpellCast::GetStaticData()
{
	return object(ptr(staticData));
}