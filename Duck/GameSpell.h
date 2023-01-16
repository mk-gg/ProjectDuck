#pragma once
#include "Memory.h"
#include "Offset.h"
#include "MemoryReadable.h"
#include "SpellInfo.h"
#include "imgui/imgui.h"

using namespace boost::python;

class GameSpell : public MemoryReadable {

public:


	std::string name;
	int         lvl;
	float       readyAt;
	float       value;

	SpellInfo* staticData;

	void ReadFromBaseAddress(int addr);
	void ImGuiDraw();
	float GetRemainingCooldown();
	object GetStaticData();

};