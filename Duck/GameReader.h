#pragma once
#include "GameState.h"
#include "Benchmark.h"


#include <queue>
#include <set>
#include <map>


class BenchmarkGameReader 
{

public:
	BenchmarkTiming readTree = BenchmarkTiming("Read Obj Tree");
	BenchmarkTiming readObjects = BenchmarkTiming("Read Game Objects");

	BenchmarkValue<int> sehExceptions = BenchmarkValue<int>("SEH Exceptions");
	BenchmarkValue<int> readsPerformed = BenchmarkValue<int>("Obj Tree Node Reads");

	BenchmarkValue<int> cacheHits = BenchmarkValue<int>("Cache Hits");
	BenchmarkValue<int> blacklistHits = BenchmarkValue<int>("Blacklist Hits");
	BenchmarkValue<int> numObjPointers = BenchmarkValue<int>("Obj Pointers Read");

	void ImGuiDraw();
};

class GameReader 
{

private:
	BenchmarkGameReader benchmark;
	
	GameState state;
	int baseAddr;
	std::set<int> blacklistedObjects;
	std::set<int> updatedObjects;


	void ReadObjectTree();
	int ReadTreeNodes(std::queue<int>& nodesToVisit, int node);
	void ReadGameObject(int address);
	void ReadLocalChampion();

	void AddToCache(GameObject* obj);
	GameObject* CreateObject(int addr);
	

public:
	GameState& GetNextState();
	BenchmarkGameReader& GetBenchmarks();

};