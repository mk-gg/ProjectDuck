#pragma once
#include <map>
#include <string>
#include <d3d9.h>
#include <memory>
#include "UnitInfo.h"
#include "ItemInfo.h"
#include "SpellInfo.h"

class LoadDataProgress
{
public:
    float spellLoadPercent = 0.0f;
    float unitsLoadPercent = 0.0f;
    float itemsLoadPercent = 0.0f;
    float imagesLoadPercent = 0.0f;

    bool  essentialsLoaded = false;
    bool  allLoaded = false;
};

class GameData
{
private:
    static const char* FolderData;
    static std::map<std::string, UnitInfo*> Units;
    static std::map<std::string, SpellInfo*> Spells;
    static std::map<int, ItemInfo*> Items;
    static std::map<std::string, PDIRECT3DTEXTURE9> Images;

	static void Load();
    static void LoadSpells(const char* fileName, float& percentValue, float percentEnd);
    static void LoadItems(const char* fileName, float& percentValue, float percentEnd);
    static void LoadUnits(const char* fileName, float& percentValue, float percentEnd);
    static void LoadImagesFromZip(const char* zipPath, float& percentValue, float percentEnd);

public:
    static std::shared_ptr<LoadDataProgress> LoadProgress;

    static void ImGuiDrawLoader();
    static void ImGuiDrawObjects();
    static void LoadAsync();
    static UnitInfo* GetUnit(std::string& str);
    static SpellInfo* GetSpell(std::string& str);
    static ItemInfo* GetItem(int id);
    static PDIRECT3DTEXTURE9 GetImage(std::string& str);

};

