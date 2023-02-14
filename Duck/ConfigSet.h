#pragma once
#include <string>
#include <map>
#include <iostream>
#include <chrono>
#include <functional>

using namespace std::chrono;
class ConfigSet
{
private:
	std::string filePath;
	std::map<std::string, std::string> rawValues;
	float saveInterval;
	high_resolution_clock::time_point timeLastSave;

public:
	ConfigSet();
	ConfigSet(std::string cfg, float saveInterval = 100);

	int GetInt(const char* key, int defaultVaul);
	bool GetBool(const char* key, bool defaultVal);
	float GetFloat(const char* key, float defaultVal);
	std::string    GetStr(const char*, const char* defaultVal);

	void SetInt(const char*, int val);
	void SetBool(const char*, bool val);
	void SetFloat(const char*, float val);
	void SetStr(const char*, const char* val);


	void SetSaveInterval(float interval);
	void SetConfigFile(const char* file);

	bool IsTimeToSave();
	void Load();
	void Save();


};

