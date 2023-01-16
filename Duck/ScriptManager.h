#pragma once

#include "Script.h"
#include "imgui/imgui.h"
#include <deque>

class ScriptManager 
{

private:
	std::deque<std::shared_ptr<Script>> scripts;

public:

	void LoadScriptsFromFolder(std::string& folderPath);
	void ExecuteScripts(PyExecutionContext& ctx);
	void ImGuiDrawMenu(PyExecutionContext& ctx);


};

