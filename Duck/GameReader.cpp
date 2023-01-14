#include "GameReader.h"
#include "Offset.h"
#include <Windows.h>
#include "Memory.h"
#include "Logger.h"
#include "imgui/imgui.h"
#include "GameUnit.h"
#include "GameData.h"
#include "GameMissile.h"
#include "GameChampion.h"


GameState& GameReader::GetNextState()
{
	benchmark.sehExceptions.value = 0;
	benchmark.cacheHits.value = 0;
	benchmark.blacklistHits.value = 0;

	baseAddr = (int)GetModuleHandle(NULL);
	memcpy(&state.time, (void*)(baseAddr + Offset::GameTime), sizeof(float));

	if (state.time > 1.f) {
		state.renderer.ReadFromBaseAddress(baseAddr);
		state.hud.ReadFromBaseAddress(baseAddr);

		ReadObjectTree();
		ReadLocalChampion();
	}
	return state;
}

BenchmarkGameReader& GameReader::GetBenchmarks()	
{
	return benchmark;
}


void GameReader::ReadLocalChampion()
{
}

void GameReader::ReadObjectTree() {

	static const int      NUM_MAX_READS = 500; /// Used to prevent infinite loops due to race conditions
	static std::set<int>  ObjectPointers;

	

	ObjectPointers.clear();
	updatedObjects.clear();

	int objManager = ReadInt(baseAddr + Offset::ObjectManager);
	int treeRoot = ReadInt(objManager + Offset::ObjectMapRoot);

	std::queue<int> nodesToVisit;
	std::set<int> visitedNodes;
	nodesToVisit.push(treeRoot);
	visitedNodes.insert(NULL);

	benchmark.readTree.Start();
	/// Read object addresses from tree
	int numObj = 0, reads = 0;
	int node;
	while (reads < NUM_MAX_READS && nodesToVisit.size() > 0) {
		node = nodesToVisit.front();
		nodesToVisit.pop();
		if (visitedNodes.find(node) != visitedNodes.end())
			continue;

		reads++;
		visitedNodes.insert(node);

		int addr = ReadTreeNodes(nodesToVisit, node);
		if (addr != NULL)
			ObjectPointers.insert(addr);

	}

	benchmark.readsPerformed.value = reads;
	benchmark.readTree.End();
	benchmark.readObjects.Start();
	benchmark.numObjPointers.value = ObjectPointers.size();
	/// Read objects using addresses read previously
	for (int ptr : ObjectPointers) {
		ReadGameObject(ptr);
	}

	/// Dispose of objects that were destroyed ingame
	auto it = state.objectCache.begin();
	while (it != state.objectCache.end()) {
		if (updatedObjects.find(it->first) == updatedObjects.end())
			it = state.objectCache.erase(it);
		it++;
	}

	benchmark.readObjects.End();
}

int GameReader::ReadTreeNodes(std::queue<int>& nodesToVisit, int node)
{
	static const int OBJ_NET_ID_START = 0x40000000;
	static const int OBJ_NET_ID_END = OBJ_NET_ID_START + 0x100000;

	__try {
		int childOne = ReadInt(node);
		int childTwo = ReadInt(node + sizeof(int));
		int childThree = ReadInt(node + 2 * sizeof(int));

		nodesToVisit.push(childOne);
		nodesToVisit.push(childTwo);
		nodesToVisit.push(childThree);

		int netId = ReadInt(node + Offset::ObjectMapNodeNetId);

		/// Check if valid object by checking if it has a valid network id
		if (netId > OBJ_NET_ID_START && netId < OBJ_NET_ID_END) {
			return ReadInt(node + Offset::ObjectMapNodeObject);
		}
	}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			benchmark.sehExceptions.value += 1;
	}
	return NULL;
}

void GameReader::AddToCache(GameObject* obj) 
{
	state.objectCache[obj->networkId] = std::shared_ptr<GameObject>(obj);
}

GameObject* GameReader::CreateObject(int addr)
{
	std::string name;
	
	int nameAddr = ReadInt(addr + Offset::ObjName);
	if (!CantRead((void*)nameAddr, 1))
		name = Memory::ReadString(nameAddr);

	if (!name.empty()) 
	{
		name = Strings::ToLower(name);
		UnitInfo* info = GameData::GetUnit(name);
		if (info == nullptr)
			return nullptr;

		if (info->HasTag(Unit_Champion))
			return new GameChampion(name);
		return new GameUnit(name);
	}

	/// Try to read missile name
	int missileSpell = ReadInt(addr + Offset::MissileSpellInfo);
	if (CantRead(missileSpell)) return nullptr;

	int missileSpellData = ReadInt(missileSpell + Offset::SpellInfoSpellData);
	if (CantRead(missileSpellData)) return nullptr;

	nameAddr = ReadInt(missileSpellData + Offset::SpellDataSpellName);
	if (CantRead(nameAddr)) return nullptr;
	name = Memory::ReadString(nameAddr);

	if (!name.empty()) {
		name = Strings::ToLower(name);
		if (GameData::GetSpell(name) == nullptr)
			return nullptr;
		return new GameMissile(name);
	}


	return nullptr;
	
}

void GameReader::ReadGameObject(int address)
{
	GameObject* obj = nullptr;
	int netId = 0;
	__try {
		int netId = ReadInt(address + Offset::ObjNetworkID);
		if (blacklistedObjects.find(netId) != blacklistedObjects.end())
		{
			benchmark.blacklistHits.value += 1;
			return;
		}
			

		auto& objectCache = state.objectCache;
		auto find = objectCache.find(netId);
		if (find == objectCache.end()) 
		{
			obj = CreateObject(address);

			/// If we can't create the object we blacklist it for performance
			if (obj == nullptr) {
				blacklistedObjects.insert(netId);
				return;
			}

			obj->ReadFromBaseAddress(address);
			AddToCache(obj);
		}
		else {
			benchmark.cacheHits.value += 1;
			obj = find->second.get();
			obj->ReadFromBaseAddress(address);
		}
		if (obj->isVisible)
			obj->lastSeen = state.time;

		updatedObjects.insert(obj->networkId);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		benchmark.sehExceptions.value += 1;
		if (obj != nullptr)
			delete obj;
	}
}



void BenchmarkGameReader::ImGuiDraw()
{
	ImGui::Text("Game Reader Benchmarks");
	ImGui::DragFloat(readTree.name, &readTree.avgMs);
	ImGui::DragFloat(readObjects.name, &readObjects.avgMs);
	ImGui::Separator();
	ImGui::DragInt(sehExceptions.name, &sehExceptions.value);
	ImGui::DragInt(readsPerformed.name, &readsPerformed.value);
	ImGui::DragInt(numObjPointers.name, &numObjPointers.value);
	ImGui::DragInt(cacheHits.name, &cacheHits.value);
	ImGui::DragInt(blacklistHits.name, &blacklistHits.value);

}