#include "Offset.h"


Offset::Offset() {};

std::string Offset::GameVersion = "12.23";

int Offset::GameTime = 0x3143C44;

int Offset::ObjIndex = 0x8;
int Offset::ObjTeam = 0x34;
int Offset::ObjNetworkID = 0xB4;
int Offset::ObjPos = 0x1DC;
int Offset::ObjVisibility = 0x274;
int Offset::ObjSpawnCount = 0x288;
int Offset::ObjSrcIndex = 0x294;
int Offset::ObjMana = 0x029C;
int Offset::ObjHealth = 0xE7C;
int Offset::ObjMaxHealth = 0xE8C;
int Offset::ObjArmor = 0x1384;
int Offset::ObjMagicRes = 0x138C;
int Offset::ObjBaseAtk = 0x135C;
int Offset::ObjBonusAtk = 0x12D4;
int Offset::ObjMoveSpeed = 0x139C;
int Offset::ObjSpellBook = 0x29C8;
int Offset::ObjName = 0x2DB4;
int Offset::ObjLvl = 0x35A4;
int Offset::ObjExpiry = 0x298;
int Offset::ObjCrit = 0x1858;
int Offset::ObjCritMulti = 0x12B8;
int Offset::ObjAbilityPower = 0x1758;
int Offset::ObjAtkSpeedMulti = 0x1358;
int Offset::ObjItemList = 0x35F0;

int Offset::ItemListItem = 0xC;
int Offset::ItemInfo = 0x20;
int Offset::ItemInfoId = 0x68;

int Offset::ViewMatrix = 0x3170CF8;
int Offset::ProjectionMatrix = ViewMatrix + 16 * sizeof(float);
int Offset::Renderer = 0x31765C0;
int Offset::RendererWidth = 0x10;
int Offset::RendererHeight = 0x14;

int Offset::SpellSlotLevel = 0x1C;
int Offset::SpellSlotTime = 0x24;
int Offset::SpellSlotValue = 0x54;
int Offset::SpellSlotSpellInfo = 0x120;
int Offset::SpellInfoSpellData = 0x40;
int Offset::SpellDataSpellName = 0x6C;
int Offset::SpellDataMissileName = 0x6C;

int Offset::ObjectManager = 0x18ADAD8;
int Offset::LocalPlayer = 0x314A404;
int Offset::UnderMouseObject = 0x24FB364;


int Offset::ObjectMapCount = 0x2C;
int Offset::ObjectMapRoot = 0x28;
int Offset::ObjectMapNodeNetId = 0x10;
int Offset::ObjectMapNodeObject = 0x14;

int Offset::MissileSpellInfo = 0x260;
int Offset::MissileSrcIdx = 0x2C4;
int Offset::MissileDestIdx = 0x314;
int Offset::MissileStartPos = 0x2E0;
int Offset::MissileEndPos = 0x2EC;

int Offset::MinimapObject = 0x3143C88;
int Offset::MinimapObjectHud = 0x15C;
int Offset::MinimapHudPos = 0x3C;
int Offset::MinimapHudSize = 0x44;