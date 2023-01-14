#pragma once
#include "MemoryReadable.h"
#include <string>
#include <windows.h>

#include "ImGuiDrawable.h"
#include "Vector.h"
#include "imgui/imgui.h"
#include "Strings.h"
#include "Offset.h"
#include "Memory.h"
#include "Logger.h"

class GameObject : MemoryReadable, ImGuiDrawable
{
public:
	std::string name;
	Vector3 pos;
	int	address;
	int networkId;
	short index;
	short team;
	bool isVisible;
	float lastSeen;


	GameObject();
	GameObject(std::string name);
	void ReadFromBaseAddress(int baseAddr);
	void ImGuiDraw();
};

