#pragma once
#include "Memory.h"
#include "Offset.h"
#include "MemoryReadable.h"
#include "SpellInfo.h"
#include "ImGuiDrawable.h"
#include "imgui/imgui.h"

class GameSpell : public MemoryReadable, public ImGuiDrawable {

public:


	std::string name;
	int         lvl;
	float       readyAt;
	float       value;

	SpellInfo* staticData;

	void ReadFromBaseAddress(int addr);
	void ImGuiDraw();
};