#pragma once

#include <iostream>

#include "GameObject.h"
#include "GameUnit.h"
#include "GameChampion.h"
#include "GameMissile.h"
#include "GameSpell.h"

#include "SpellInfo.h"
#include "ItemInfo.h"
#include "UnitInfo.h"

#include "GameState.h"
#include "Color.h"

#include "PyExecutionContext.h"

void greet()
{
    std::cout << "zz Implemented Offsetscanner ";
}


BOOST_PYTHON_MODULE(Duck)
{
    using namespace boost::python;
	class_<UnitInfo>("UnitStatic", "Static data loaded at runtime for a unit")
		.def_readonly("hp_bar_height", &UnitInfo::healthBarHeight)
		.def_readonly("movement_speed", &UnitInfo::baseMovementSpeed)
		.def_readonly("base_atk_range", &UnitInfo::baseAttackRange)
		.def_readonly("base_atk_speed", &UnitInfo::baseAttackSpeed)
		.def_readonly("atk_speed_ratio", &UnitInfo::attackSpeedRatio)

		.def_readonly("acquisition_radius", &UnitInfo::acquisitionRange)
		.def_readonly("selection_radius", &UnitInfo::selectionRadius)
		.def_readonly("pathing_radius", &UnitInfo::pathRadius)
		.def_readonly("gameplay_radius", &UnitInfo::gameplayRadius)

		.def_readonly("basic_atk", &UnitInfo::GetBasicAttack)
		.def_readonly("basic_atk_windup", &UnitInfo::basicAttackWindup)
		.def_readonly("basic_atk_cast_time", &UnitInfo::basicAttackCastTime)
		;

	class_<ItemInfo>("ItemStatic", "Static data loaded at runtime for an item")
		.def_readonly("id", &ItemInfo::id)
		.def_readonly("cost", &ItemInfo::cost)
		.def_readonly("mov_speed", &ItemInfo::movementSpeed)
		.def_readonly("health", &ItemInfo::health)
		.def_readonly("crit", &ItemInfo::crit)
		.def_readonly("ap", &ItemInfo::abilityPower)
		.def_readonly("mana", &ItemInfo::mana)
		.def_readonly("armour", &ItemInfo::armour)
		.def_readonly("magic_resist", &ItemInfo::magicResist)
		.def_readonly("phys_damage", &ItemInfo::physicalDamage)
		.def_readonly("attack_speed", &ItemInfo::attackSpeed)
		.def_readonly("life_steal", &ItemInfo::lifeSteal)
		.def_readonly("hp_regen", &ItemInfo::hpRegen)
		.def_readonly("mov_speed_percent", &ItemInfo::movementSpeedPercent)
		;

	class_<SpellInfo>("SpellStatic", "Static data loaded at runtime for a spell")
		.def_readonly("icon", &SpellInfo::icon)
		.def_readonly("cast_time", &SpellInfo::castTime)
		.def_readonly("cast_range", &SpellInfo::castRange)
		.def_readonly("cast_radius", &SpellInfo::castRadius)
		.def_readonly("width", &SpellInfo::width)
		.def_readonly("height", &SpellInfo::height)
		.def_readonly("speed", &SpellInfo::speed)
		.def_readonly("travel_time", &SpellInfo::travelTime)
		;

	class_<GameObject>("Obj", "Represents the base of a ingame object. Most ingame objects derive from this.")
		.def_readonly("name", &GameObject::name)
		.def_readonly("index", &GameObject::index)
		.def_readonly("net_id", &GameObject::networkId)
		.def_readonly("team", &GameObject::team)
		.def_readonly("pos", &GameObject::pos)
		.def_readonly("visible", &GameObject::isVisible)
		.def_readonly("last_seen", &GameObject::lastSeen)

		.def("ally_to", &GameObject::IsAllyTo)
		.def("enemy_to", &GameObject::IsEnemyTo)
		.def("__eq__", &GameObject::EqualsTo)
		;

	class_<GameSpell>("Spell", "Spell in-game")
		.def_readonly("name", &GameSpell::name)
		.def_readonly("lvl", &GameSpell::lvl)
		.def_readonly("ready_at", &GameSpell::readyAt)
		.def_readonly("value", &GameSpell::value)
		.def_readonly("cd", &GameSpell::GetRemainingCooldown)
		.def_readonly("static", &GameSpell::GetStaticData)
		;

	class_<SpellCast>("SpellCast", "Has data about a spell cast.")
		.def_readonly("start_pos", &SpellCast::start)
		.def_readonly("end_pos", &SpellCast::end)
		.def_readonly("src_index", &SpellCast::srcIndex)
		.def_readonly("dest_index", &SpellCast::destIndex)
		.def_readonly("time_begin", &SpellCast::timeBegin)
		.def_readonly("cast_time", &SpellCast::castTime)
		.def_readonly("static", &SpellCast::GetStaticData)
		;

	class_<GameMissile, bases<GameObject>>("Missile", "Represent a missile object.")
		.def_readonly("spell", &GameMissile::GetSpell)
		;

	class_<GameUnit, bases<GameObject>>("Unit", "Unit object in-game")
		.def_readonly("dead", &GameUnit::isDead)
		.def_readonly("mana", &GameUnit::mana)
		.def_readonly("health", &GameUnit::health)
		.def_readonly("max_health", &GameUnit::maxHealth)
		.def_readonly("armor", &GameUnit::armor)
		.def_readonly("magic_res", &GameUnit::magicRes)
		.def_readonly("base_atk", &GameUnit::baseAtk)
		.def_readonly("bonus_atk", &GameUnit::bonusAtk)
		.def_readonly("move_speed", &GameUnit::moveSpeed)
		.def_readonly("lvl", &GameUnit::lvl)
		.def_readonly("expiry", &GameUnit::expiry)
		.def_readonly("crit", &GameUnit::crit)
		.def_readonly("crit_multi", &GameUnit::critMulti)
		.def_readonly("ap", &GameUnit::abilityPower)
		.def_readonly("atk_speed_multi", &GameUnit::atkSpeedMulti)
		.def_readonly("atk_range", &GameUnit::attackRange)
		.def_readonly("atk_speed", &GameUnit::GetAttackSpeed)
		.def_readonly("static", &GameUnit::GetStaticData)
		.def("has_tags", &GameUnit::HasTags)
		;

	class_<GameChampion, bases<GameUnit>>("Champion", "Champions in-game")
		.def_readonly("spells", &GameChampion::SpellsToPy)
		.def_readonly("items", &GameChampion::ItemsToPy)
		.def_readonly("hpbar_pos", &GameChampion::GetHpBarPosition)
		;

	class_<GameTurret, bases<GameUnit>>("Turret", "Represents a turret object")
		;

	class_<GameMinion, bases<GameUnit>>("Minion", "Represents a minion object")
		;

	class_<GameJungle, bases<GameUnit>>("JungleMob", "Represents a jungle mob object")
		;

	class_<PyImGui>("UI")
		.def("begin", &PyImGui::Begin)
		.def("end", &PyImGui::End)

		.def("button", &PyImGui::Button)
		.def("colorbutton", &PyImGui::ColorButton)
		.def("colorpick", &PyImGui::ColorPicker)
		.def("checkbox", &PyImGui::Checkbox)
		.def("text", &PyImGui::Text)
		.def("text", &PyImGui::TextColored)
		.def("labeltext", &PyImGui::LabelText)
		.def("labeltext", &PyImGui::LabelTextColored)
		.def("separator", &PyImGui::Separator)
		.def("dragint", &PyImGui::DragInt, PyImGui::DragIntOverloads())
		.def("dragfloat", &PyImGui::DragFloat, PyImGui::DragFloatOverloads())
		.def("keyselect", &PyImGui::KeySelect)
		.def("sliderfloat", &PyImGui::SliderFloat)

		.def("image", &PyImGui::Image)

		.def("header", &PyImGui::CollapsingHeader)
		.def("treenode", &PyImGui::TreeNode)
		.def("treepop", &PyImGui::TreePop)
		.def("opennext", &PyImGui::SetNextItemOpen)

		.def("sameline", &PyImGui::SameLine)
		.def("begingroup", &PyImGui::BeginGroup)
		.def("endgroup", &PyImGui::EndGroup)

		.def("listbox", &PyImGui::ListBox)
		.def("combo", &PyImGui::Combo)
		;

	class_<PyExecutionContext>("Context", "Contains everything necessarry to create scripts. From utility functions to game data")
		.def("log", &PyExecutionContext::Log, "Logs a message in the Duck Console")
		.def_readonly("ui", &PyExecutionContext::GetImGuiInterface, "UI interface for drawing menus based on imgui")
		.def_readonly("cfg", &PyExecutionContext::GetConfig, "Used to load/save script specific configs")
		.def_readonly("time", &PyExecutionContext::time, "Current game duration in seconds")
		.def_readonly("hovered", &PyExecutionContext::hovered, "Game object under the mouse")
		.def_readonly("player", &PyExecutionContext::player, "The champion used by the local player")

		.def_readonly("champs", &PyExecutionContext::GetChampions)
		.def_readonly("turrets", &PyExecutionContext::GetTurrets)
		.def_readonly("missiles", &PyExecutionContext::GetMissiles)
		.def_readonly("minions", &PyExecutionContext::GetMinions)
		.def_readonly("jungle", &PyExecutionContext::GetJungle)
		.def_readonly("others", &PyExecutionContext::GetOthers)
		.def("attack", &PyExecutionContext::AttackUnit)
		.def("move", &PyExecutionContext::MoveToLocation)
		.def("move", &PyExecutionContext::MoveToMouse)

		.def("is_held", &PyExecutionContext::IsKeyDown)

		.def("is_on_screen", &PyExecutionContext::IsScreenPointOnScreen, PyExecutionContext::IsScreenPointOnScreenOverloads())
		.def("is_on_screen", &PyExecutionContext::IsWorldPointOnScreen, PyExecutionContext::IsWorldPointOnScreenOverloads())
		.def("w2s", &PyExecutionContext::World2Screen)
		.def("w2m", &PyExecutionContext::World2Minimap)
		.def("d2m", &PyExecutionContext::DistanceOnMinimap)

		.def("line", &PyExecutionContext::DrawLine)
		.def("circle", &PyExecutionContext::DrawCircle)
		.def("circle_fill", &PyExecutionContext::DrawCircleFilled)
		.def("circle", &PyExecutionContext::DrawCircleWorld)
		.def("circle_fill", &PyExecutionContext::DrawCircleWorldFilled)
		.def("text", &PyExecutionContext::DrawTxt)
		.def("rect", &PyExecutionContext::DrawRect, PyExecutionContext::DrawRectOverloads())
		.def("rect_fill", &PyExecutionContext::DrawRectFilled, PyExecutionContext::DrawRectFilledOverloads())
		.def("rect", &PyExecutionContext::DrawRectWorld)
		.def("triangle", &PyExecutionContext::DrawTriangleWorld)
		.def("triangle_fill", &PyExecutionContext::DrawTriangleWorldFilled)
		.def("image", &PyExecutionContext::DrawImage)
		.def("image", &PyExecutionContext::DrawImageRounded)
		;


	class_<ConfigSet>("Config")
		.def("set_int", &ConfigSet::SetInt)
		.def("set_bool", &ConfigSet::SetBool)
		.def("set_float", &ConfigSet::SetFloat)
		.def("set_str", &ConfigSet::SetStr)
		.def("get_int", &ConfigSet::GetInt)
		.def("get_bool", &ConfigSet::GetBool)
		.def("get_float", &ConfigSet::GetFloat)
		.def("get_str", &ConfigSet::GetStr)
		;

	class_<ImVec4>("Col", init<float, float, float, float>())
		.def_readonly("Black", &Color::BLACK)
		.def_readonly("White", &Color::WHITE)
		.def_readonly("Red", &Color::RED)
		.def_readonly("DarkRed", &Color::DARK_RED)
		.def_readonly("Green", &Color::GREEN)
		.def_readonly("DarkGreen", &Color::DARK_GREEN)
		.def_readonly("Yellow", &Color::YELLOW)
		.def_readonly("DarkYellow", &Color::DARK_YELLOW)
		.def_readonly("Cyan", &Color::CYAN)
		.def_readonly("Purple", &Color::PURPLE)
		.def_readonly("Gray", &Color::GRAY)
		.def_readonly("Orange", &Color::ORANGE)
		.def_readonly("Blue", &Color::BLUE)
		.def_readonly("Brown", &Color::BROWN)

		.def_readwrite("r", &ImVec4::x)
		.def_readwrite("g", &ImVec4::y)
		.def_readwrite("b", &ImVec4::z)
		.def_readwrite("a", &ImVec4::w)
		;

	class_<Vector4>("Vec4", init<float, float, float, float>())
		.def_readwrite("x", &Vector4::x)
		.def_readwrite("y", &Vector4::y)
		.def_readwrite("z", &Vector4::z)
		.def_readwrite("w", &Vector4::w)
		.def("length", &Vector4::length)
		.def("normalize", &Vector4::normalize)
		.def("distance", &Vector4::distance)
		.def("__mul__", &Vector4::scale)
		.def("__mul__", &Vector4::vscale)
		.def("__add__", &Vector4::add)
		.def("__sub__", &Vector4::sub)
		.def("clone", &Vector4::clone)
		;

	class_<Vector3>("Vec3", init<float, float, float>())
		.def_readwrite("x", &Vector3::x)
		.def_readwrite("y", &Vector3::y)
		.def_readwrite("z", &Vector3::z)
		.def("length", &Vector3::length)
		.def("normalize", &Vector3::normalize)
		.def("distance", &Vector3::distance)
		.def("__mul__", &Vector3::scale)
		.def("__mul__", &Vector3::vscale)
		.def("rotate_x", &Vector3::rotate_x)
		.def("rotate_y", &Vector3::rotate_y)
		.def("rotate_z", &Vector3::rotate_z)
		.def("__add__", &Vector3::add)
		.def("__sub__", &Vector3::sub)
		.def("clone", &Vector3::clone)
		;

	class_<Vector2>("Vec2", init<float, float>())
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)
		.def("length", &Vector2::length)
		.def("normalize", &Vector2::normalize)
		.def("distance", &Vector2::distance)
		.def("__mul__", &Vector2::scale)
		.def("__mul__", &Vector2::vscale)
		.def("__add__", &Vector2::add)
		.def("__sub__", &Vector2::sub)
		.def("clone", &Vector2::clone)
		;

	enum_<UnitTag>("Unit")
		.value("Champion", UnitTag::Unit_Champion)
		.value("ChampionClone", UnitTag::Unit_Champion_Clone)
		.value("IsolationNonImpacting", UnitTag::Unit_IsolationNonImpacting)
		.value("KingPoro", UnitTag::Unit_KingPoro)
		.value("Minion", UnitTag::Unit_Minion)
		.value("MinionLane", UnitTag::Unit_Minion_Lane)
		.value("MinionLaneMelee", UnitTag::Unit_Minion_Lane_Melee)
		.value("MinionLaneRanged", UnitTag::Unit_Minion_Lane_Ranged)
		.value("MinionLaneSiege", UnitTag::Unit_Minion_Lane_Siege)
		.value("MinionLaneSuper", UnitTag::Unit_Minion_Lane_Super)
		.value("MinionSummon", UnitTag::Unit_Minion_Summon)
		.value("MinionSummon_Large", UnitTag::Unit_Minion_Summon_Large)
		.value("Monster", UnitTag::Unit_Monster)
		.value("MonsterBlue", UnitTag::Unit_Monster_Blue)
		.value("MonsterBuff", UnitTag::Unit_Monster_Buff)
		.value("MonsterCamp", UnitTag::Unit_Monster_Camp)
		.value("MonsterCrab", UnitTag::Unit_Monster_Crab)
		.value("MonsterDragon", UnitTag::Unit_Monster_Dragon)
		.value("MonsterEpic", UnitTag::Unit_Monster_Epic)
		.value("MonsterGromp", UnitTag::Unit_Monster_Gromp)
		.value("MonsterKrug", UnitTag::Unit_Monster_Krug)
		.value("MonsterLarge", UnitTag::Unit_Monster_Large)
		.value("MonsterMedium", UnitTag::Unit_Monster_Medium)
		.value("MonsterRaptor", UnitTag::Unit_Monster_Raptor)
		.value("MonsterRed", UnitTag::Unit_Monster_Red)
		.value("MonsterWolf", UnitTag::Unit_Monster_Wolf)
		.value("Plant", UnitTag::Unit_Plant)
		.value("Special", UnitTag::Unit_Special)
		.value("SpecialAzirR", UnitTag::Unit_Special_AzirR)
		.value("SpecialAzirW", UnitTag::Unit_Special_AzirW)
		.value("SpecialCorkiBomb", UnitTag::Unit_Special_CorkiBomb)
		.value("SpecialEpicMonsterIgnores", UnitTag::Unit_Special_EpicMonsterIgnores)
		.value("SpecialKPMinion", UnitTag::Unit_Special_KPMinion)
		.value("SpecialMonsterIgnores", UnitTag::Unit_Special_MonsterIgnores)
		.value("SpecialPeaceful", UnitTag::Unit_Special_Peaceful)
		.value("SpecialSyndraSphere", UnitTag::Unit_Special_SyndraSphere)
		.value("SpecialTeleportTarget", UnitTag::Unit_Special_TeleportTarget)
		.value("SpecialTrap", UnitTag::Unit_Special_Trap)
		.value("SpecialTunnel", UnitTag::Unit_Special_Tunnel)
		.value("SpecialTurretIgnores", UnitTag::Unit_Special_TurretIgnores)
		.value("SpecialUntargetableBySpells", UnitTag::Unit_Special_UntargetableBySpells)
		.value("SpecialVoid", UnitTag::Unit_Special_Void)
		.value("SpecialYorickW", UnitTag::Unit_Special_YorickW)
		.value("Structure", UnitTag::Unit_Structure)
		.value("StructureInhibitor", UnitTag::Unit_Structure_Inhibitor)
		.value("StructureNexus", UnitTag::Unit_Structure_Nexus)
		.value("StructureTurret", UnitTag::Unit_Structure_Turret)
		.value("StructureTurretInhib", UnitTag::Unit_Structure_Turret_Inhib)
		.value("StructureTurretInner", UnitTag::Unit_Structure_Turret_Inner)
		.value("StructureTurretNexus", UnitTag::Unit_Structure_Turret_Nexus)
		.value("StructureTurretOuter", UnitTag::Unit_Structure_Turret_Outer)
		.value("StructureTurretShrine", UnitTag::Unit_Structure_Turret_Shrine)
		.value("Ward", UnitTag::Unit_Ward)
		;

    def("greet", greet);

}