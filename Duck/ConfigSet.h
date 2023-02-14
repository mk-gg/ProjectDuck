#pragma once
#include <string>
#include <map>
#include <iostream>
class ConfigSet
{
private:
	std::string filePath;
	std::map<std::string, std::string> rawValues;

public:
	ConfigSet(std::string path) {};

	int GetInt(const char* key, int defaultVaul);
	bool GetBool(const char* key, bool defaultVal);
	float GetFloat(const char* key, float defaultVal);
	std::string    GetStr(const char*, const char* defaultVal);

	void SetInt(const char*, int val);
	void SetBool(const char*, bool val);
	void SetFloat(const char*, float val);
	void SetStr(const char*, const char* val);

	void Load();
	void Save();


};

