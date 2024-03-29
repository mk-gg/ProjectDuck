#pragma once
#include <string>
class Offset
{
public:
	Offset();
	static std::string GameVersion;

	static int GameTime;
	static int PlayerName;

	static int ObjIndex;
	static int ObjTeam;
	static int ObjNetworkID;
	static int ObjPos;
	static int ObjVisibility;
	static int ObjSpawnCount;
	static int ObjHealth;
	static int ObjMaxHealth;
	static int ObjMana;
	static int ObjMaxMana;
	static int ObjArmor;
	static int ObjBonusArmor;
	static int ObjMagicRes;
	static int ObjBonusMagicRes;
	static int ObjBaseAtk;
	static int ObjBonusAtk;
	static int ObjMoveSpeed;
	static int ObjSpellBook;
	static int ObjRecallState;
	static int ObjTransformation;
	static int ObjName;
	static int ObjLvl;
	static int ObjExpiry;
	static int ObjCrit;
	static int ObjCritMulti;
	static int ObjAbilityPower;
	static int ObjAtkSpeedMulti;
	static int ObjAtkRange;
	static int ObjItemList;
	static int ObjMissileSpellCast;
	static int ObjMissileName;
	static int ObjTargetable;
	static int ObjInvulnerable;

	static int ItemListItem;
	static int ItemInfo;
	static int ItemInfoId;

	static int ViewMatrix;
	static int ProjectionMatrix;
	static int Renderer;
	static int RendererWidth;
	static int RendererHeight;

	static int SpellSlotLevel;
	static int SpellSlotTime;
	static int SpellSlotValue;
	static int SpellSlotSpellInfo;
	static int SpellInfoSpellData;
	static int SpellDataSpellName;
	static int SpellDataMissileName;

	static int ObjectManager;
	static int LocalPlayer;
	static int UnderMouseObject;

	static int ObjectMapCount;
	static int ObjectMapRoot;
	static int ObjectMapNodeNetId;
	static int ObjectMapNodeObject;


	static int MinimapObject;
	static int MinimapObjectHud;
	static int MinimapHudPos;
	static int MinimapHudSize;

	static int Chat;
	static int ChatIsOpen;


	static int SpellBookActiveSpellCast;
	static int SpellBookSpellSlots;

	static int SpellCastSpellInfo;
	static int SpellCastStartTime;
	static int SpellCastCastTime;
	static int SpellCastEnd;
	static int SpellCastStart;
	static int SpellCastSrcIdx;
	static int SpellCastDestIdx;
};

