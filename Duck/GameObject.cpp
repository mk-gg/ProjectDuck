
#include "GameObject.h"

GameObject::GameObject()
{
	this->name = std::string("Unknown");
	this->type = OBJ_UNKNOWN;
}

GameObject::GameObject(std::string name)
{
	this->name = name;
	this->type = OBJ_UNKNOWN;

}

void GameObject::ReadFromBaseAddress(int baseAddr) {
	address = baseAddr;
	networkId = ReadInt(baseAddr + Offset::ObjNetworkID);


	index = ReadShort(baseAddr + Offset::ObjIndex);
	team = ReadShort(baseAddr + Offset::ObjTeam);
	isVisible = ReadBool(baseAddr + Offset::ObjVisibility);
	memcpy(&pos, AsPtr(baseAddr + Offset::ObjPos), sizeof(Vector3));
}

bool GameObject::IsAllyTo(const GameObject& other)
{
	return other.team == this->team;
}

bool GameObject::IsEnemyTo(const GameObject& other)
{
	return other.team != this->team;
}

bool GameObject::EqualsTo(const GameObject& other)
{
	return this->networkId == other.networkId;
}

void GameObject::ImGuiDraw()
{
	int idx = index;
	int tm = team;

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 0.4f, 1.f), name.c_str());
	pos.ImGuiDraw("Position");
	ImGui::DragInt("Address", &address, 1.f, 0, 0, "%#010x");
	ImGui::DragInt("NetworkId", &networkId, 1.f, 0, 0, "%#010x");
	ImGui::DragInt("Index", &idx);
	ImGui::DragInt("Team", &tm);
	ImGui::DragFloat("LastSeen", &lastSeen);
	ImGui::Checkbox("Visible", &isVisible);
}