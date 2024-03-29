#include "Offset.h"


Offset::Offset() {};

std::string Offset::GameVersion = "13.1";

int Offset::GameTime = 0x316FDE4; //

int Offset::ObjIndex = 0x08; 
int Offset::ObjTeam = 0x34;  //NC
int Offset::ObjNetworkID = 0xB4;  //NC
int Offset::ObjPos = 0x1DC; 
int Offset::ObjVisibility = 0x274; //0x274 
int Offset::ObjSpawnCount = 0x288; 
int Offset::ObjMana = 0x29C; //NC
int Offset::ObjMaxMana = 0x2AC; //NC
int Offset::ObjHealth = 0xE7C; // NC
int Offset::ObjMaxHealth = 0xE8C; // NC
int Offset::ObjArmor = 0x1384; // NC
int Offset::ObjBonusArmor = 0x1388;
int Offset::ObjMagicRes = 0x138C;
int Offset::ObjBonusMagicRes = 0x1390;
int Offset::ObjBaseAtk = 0x135C; //NC
int Offset::ObjBonusAtk = 0x12D4; //NC
int Offset::ObjMoveSpeed = 0x139C;
int Offset::ObjSpellBook = 0x2500; //
int Offset::ObjName = 0x2DB4; //
int Offset::ObjLvl = 0x35A4; // 
int Offset::ObjExpiry = 0x298;
int Offset::ObjCrit = 0x1858;
int Offset::ObjCritMulti = 0x12D4;
int Offset::ObjAbilityPower = 0x1758;
int Offset::ObjAtkRange = 0x13A4; // NC
int Offset::ObjAtkSpeedMulti = 0x132C;
int Offset::ObjItemList = 0x35f0; //
int Offset::ObjMissileName = 0x54;
int Offset::ObjMissileSpellCast = 0x250;
int Offset::ObjTargetable = 0xD04;
int Offset::ObjInvulnerable = 0x3D4;

int Offset::ItemListItem = 0xC;
int Offset::ItemInfo = 0x20;
int Offset::ItemInfoId = 0x74;

int Offset::ViewMatrix = 0x319CF18; //
int Offset::ProjectionMatrix = ViewMatrix + 16 * sizeof(float);
int Offset::Renderer = 0x31A28B8; //
int Offset::RendererWidth = 0x8; 
int Offset::RendererHeight = 0xc;  

int Offset::SpellSlotLevel = 0x1C;
int Offset::SpellSlotTime = 0x24;
int Offset::SpellSlotValue = 0x94;
int Offset::SpellSlotSpellInfo = 0x120;
int Offset::SpellInfoSpellData = 0x40;
int Offset::SpellDataSpellName = 0x6C;
int Offset::SpellDataMissileName = 0x6C;

int Offset::ObjectManager = 0x18D9ACC; //
int Offset::LocalPlayer = 0x31767BC; //
int Offset::UnderMouseObject = 0x2514404; 

// NC
int Offset::ObjectMapCount = 0x2C;
int Offset::ObjectMapRoot = 0x28;
int Offset::ObjectMapNodeNetId = 0x10;
int Offset::ObjectMapNodeObject = 0x14;


int Offset::MinimapObject = 0x316FE24; //
int Offset::MinimapObjectHud = 0x160; //
int Offset::MinimapHudPos = 0x3C; // 
int Offset::MinimapHudSize = 0x44;

int Offset::Chat = 0x31768A8;
int Offset::ChatIsOpen = 0x814;

int Offset::SpellBookActiveSpellCast = 0x20;
int Offset::SpellBookSpellSlots = 0x4C8;

int Offset::SpellCastSpellInfo = 0x8;
int Offset::SpellCastStartTime = 0x15C;
int Offset::SpellCastCastTime = 0x4C0;
int Offset::SpellCastStart = 0x80 + 0x4;
int Offset::SpellCastEnd = 0x8C + 0x4;
int Offset::SpellCastSrcIdx = 0x68;
int Offset::SpellCastDestIdx = 0xC0;