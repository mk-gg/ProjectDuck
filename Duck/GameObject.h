#pragma once
#include "MemoryReadable.h"
#include <string>
#include <windows.h>

#include "Vector.h"
#include "imgui/imgui.h"
#include "Strings.h"
#include "Offset.h"
#include "Memory.h"
#include "Logger.h"

enum GameObjectType 
{

	OBJ_UNKNOWN,
	OBJ_CHAMPION,
	OBJ_MINION,
	OBJ_JUNGLE,
	OBJ_TURRET,
	OBJ_MISSILE
};

class GameObject : MemoryReadable
{
public:
	GameObjectType type;
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
	bool IsAllyTo(const GameObject& other);
	bool IsEnemyTo(const GameObject& other);
	bool EqualsTo(const GameObject& other);
	virtual void ImGuiDraw();
};

