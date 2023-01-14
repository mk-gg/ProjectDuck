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
    bool complete = false;
    float percentDone = 0.0f;
    const char* currentlyLoading = "Loading not started";
};

class GameData
{
private:
    static const char* FolderData;
	static void Load();
    static void LoadSpells(const char* fileName, float percentEnd);
    static void LoadItems(const char* fileName, float percentEnd);
    static void LoadUnits(const char* fileName, float percentEnd);
    static void LoadImages(const char* folderName, float percentEnd);
    static void LoadImagesFromZip(const char* zipPath, float percentEnd);

public:
    static std::shared_ptr<LoadDataProgress> LoadProgress;
    static std::map<std::string, UnitInfo*> Units;
    static std::map<std::string, SpellInfo*> Spells;
    static std::map<int, ItemInfo*> Items;
    static std::map<std::string, PDIRECT3DTEXTURE9*> Images;
    static void LoadAsync();

};

