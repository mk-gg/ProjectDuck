#pragma once
#include <sstream>
#include <fstream>
#include <mutex>
#include <list>

class Logger
{
private:
	std::shared_ptr<std::iostream> stream;
	std::mutex streamMutex;

public:
	static Logger File;
	static Logger Console;
	Logger(std::shared_ptr<std::iostream> stream);

	void Log(const char* str, ...);
	void GetLines(std::list<std::string>& lines);
	static void LogAll(const char* str, ...);
};
