#include "GameChampion.h"
#include "GameData.h"


GameChampion::GameChampion()
{
}

GameChampion::GameChampion(std::string name)
	:GameUnit(name)
{
}

void GameChampion::ReadFromBaseAddress(int addr)
{
	GameUnit::ReadFromBaseAddress(addr);

	/// Read spells
	int spellBook = addr + Offset::ObjSpellBook;
	for (int i = 0; i < 6; ++i) {
		spells[i].ReadFromBaseAddress(ReadInt(spellBook + i * sizeof(int)));
	}

	/// Read items
	int itemList = ReadInt(addr + Offset::ObjItemList);
	for (int i = 0; i < 6; ++i) {
		items[i] = nullptr;

		int item = ReadInt(itemList + i * 0x10 + Offset::ItemListItem);
		if (item == 0)
			continue;

		int itemInfo = ReadInt(item + Offset::ItemInfo);
		if (itemInfo == 0)
			continue;

		int id = ReadInt(itemInfo + Offset::ItemInfoId);
		items[i] = GameData::GetItem(id);
	}
}

void GameChampion::ImGuiDraw()
{
	GameUnit::ImGuiDraw();
	ImGui::Separator();

	ImGui::Text("Spells");
	for (int i = 0; i < 6; ++i) {
		if (ImGui::TreeNode(spells[i].name.c_str())) {

			spells[i].ImGuiDraw();
			ImGui::TreePop();
		}
	}

	ImGui::Text("Items");
	for (int i = 0; i < 6; ++i) {
		if (items[i] == nullptr)
			continue;

		if (ImGui::TreeNode(Strings::Format("%d", items[i]->id).c_str())) {
			ImGui::TreePop();
		}
	}
}


object GameChampion::SpellsToPy()
{
	list l;
	for (int i = 0; i < 6; ++i)
		l.append(boost::ref(spells[i]));

	return l;
}

object GameChampion::ItemsToPy()
{
	list l;
	for (int i = 0; i < 6; ++i)
		l.append(ptr(items[i]));

	return l;
}