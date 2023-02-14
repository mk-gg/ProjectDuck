#include "Globals.h"
#include <fstream>



fs::path FindWorkingDir()
{
	char* buff;
	size_t buffCount;
	_dupenv_s(&buff, &buffCount, "VPath");
	return fs::path(buff);

}

std::string GetImGuiIniPath() {

	fs::path path = Globals::WorkingDir;
	path /= "imgui.ini";

	return path.u8string();
}

fs::path GetConfigsDir() {

	fs::path path = Globals::WorkingDir;
	path /= "configs";

	return path;
}

fs::path Globals::WorkingDir = fs::path("C:\\Users\\Gramps\\source\\repos\\ProjectDuck\\Deployable");
//fs::path Globals::WorkingDir = FindWorkingDir();
fs::path Globals::ConfigsDir = GetConfigsDir();
std::string Globals::ImGuiIniPath = GetImGuiIniPath();

