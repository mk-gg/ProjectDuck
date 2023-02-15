#pragma once
#include "PyExecutionContext.h"
#include <string>
#include "ConfigSet.h"
#include "InputController.h"

using namespace boost::python;

enum ScriptFunction
{
	ON_LOOP = 0,
	ON_MENU = 1,
	ON_LOAD = 2,
	ON_SAVE = 3
};

class Script 
{

private:

	bool         LoadFunc(PyObject** loadInto, const char* funcName);
	bool         LoadInfo();

	PyObject* moduleObj;
	PyObject* functions[4];

public:
	Script();
	~Script();
	bool LoadFromFile(std::string& filePath);
	void Execute(PyExecutionContext& ctx, ScriptFunction func);
	static std::string GetPyError();

	bool loaded;
	bool neverExecuted;


	std::string  error;
	std::string  fileName;
	std::string  prettyName;
	std::string  author;
	std::string  description;
	std::string  targetChamp;
	std::string  icon;

	ConfigSet config;
	InputController input;

};

