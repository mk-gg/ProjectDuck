#include "Offset.h"


Offset::Offset() {};

std::string Offset::GameVersion = "13.1";

int Offset::GameTime = 0x315CCF4; //

int Offset::ObjIndex = 0x08; //
int Offset::ObjTeam = 0x34; //
int Offset::ObjNetworkID = 0xB4; //
int Offset::ObjPos = 0x1DC; //
int Offset::ObjVisibility = 0x274; //
int Offset::ObjSpawnCount = 0x288; //
int Offset::ObjSrcIndex = 0x294;
int Offset::ObjMana = 0x029C;
int Offset::ObjMaxMana = 0x2AC;
int Offset::ObjHealth = 0xE7C;
int Offset::ObjMaxHealth = 0xE8C;
int Offset::ObjArmor = 0x1384;
int Offset::ObjBonusArmor = 0x1388;
int Offset::ObjMagicRes = 0x138C;
int Offset::ObjBonusMagicRes = 0x1390;
int Offset::ObjBaseAtk = 0x135C;
int Offset::ObjBonusAtk = 0x12D4;
int Offset::ObjMoveSpeed = 0x139C;
int Offset::ObjSpellBook = 0x29D0;
int Offset::ObjName = 0x2DBC;
int Offset::ObjLvl = 0x35AC;
int Offset::ObjExpiry = 0x298;
int Offset::ObjCrit = 0x1858;
int Offset::ObjCritMulti = 0x12D4;
int Offset::ObjAbilityPower = 0x1758;
int Offset::ObjAtkSpeedMulti = 0x132C;
int Offset::ObjItemList = 0x35F8;

int Offset::ItemListItem = 0xC;
int Offset::ItemInfo = 0x20;
int Offset::ItemInfoId = 0x74;

int Offset::ViewMatrix = 0x3189D00;
int Offset::ProjectionMatrix = ViewMatrix + 16 * sizeof(float);
int Offset::Renderer = 0x318F6A0; //
int Offset::RendererWidth = 0x8; //
int Offset::RendererHeight = 0xc; // 

int Offset::SpellSlotLevel = 0x1C;
int Offset::SpellSlotTime = 0x24;
int Offset::SpellSlotValue = 0x94;
int Offset::SpellSlotSpellInfo = 0x120;
int Offset::SpellInfoSpellData = 0x40;
int Offset::SpellDataSpellName = 0x6C;
int Offset::SpellDataMissileName = 0x6C;

int Offset::ObjectManager = 0x18C6B1C; //
int Offset::LocalPlayer = 0x3163080; //
int Offset::UnderMouseObject = 0x2514404; //


int Offset::ObjectMapCount = 0x2C;
int Offset::ObjectMapRoot = 0x28;
int Offset::ObjectMapNodeNetId = 0x10;
int Offset::ObjectMapNodeObject = 0x14;

int Offset::MissileSpellInfo = 0x260;
int Offset::MissileSrcIdx = 0x2C4;
int Offset::MissileDestIdx = 0x318; // 0x314
int Offset::MissileStartPos = 0x2E0;
int Offset::MissileEndPos = 0x2EC;

int Offset::MinimapObject = 0x3143C88;
int Offset::MinimapObjectHud = 0x15C;
int Offset::MinimapHudPos = 0x3C;
int Offset::MinimapHudSize = 0x44;