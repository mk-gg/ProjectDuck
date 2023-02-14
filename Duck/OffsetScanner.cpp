#include "OffsetScanner.h"
#include "Color.h"
#include "Logger.h"
#include "Strings.h"

#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <iostream>

char                         OffsetScanner::CodeDump[2048] = { 0 };
bool                         OffsetScanner::Scanning = false;

/*
Renderer
ViewMatrix
MinimapObj
*/

std::vector<OffsetSignature> OffsetScanner::signatures = std::vector<OffsetSignature>({
	OffsetSignature("ObjectManager",     "8B 0D ? ? ? ? E8 ? ? ? ? FF 77",                   2), 
	OffsetSignature("Renderer",          "8B 15 ? ? ? ? 83 EC 08 F3",                        2), 
	OffsetSignature("ViewMatrix",        "B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E9 ? ? ? ?",      2),
	OffsetSignature("MinimapObject",     "FF 52 04 8B 0D ? ? ? ? E8 ? ? ? ?",                5),
	OffsetSignature("LocalPlayer",       "8B 3D ? ? ? ? 3B F7 75 09", 2),
	OffsetSignature("GameTime",          "F3 0F 11 05 ? ? ? ? 8B 49",                        4),

	});

void OffsetScanner::ImGuiDraw()
{
	ImGui::Begin("Offset scanner");
	if (ImGui::Button("Scan Signatures") && !Scanning) {
		Scanning = true;
		std::thread scanner(Scan);
		scanner.detach();
	}

	ImGui::SameLine();
	if (ImGui::Button("Dump Signatures")) {
		std::string code;
		for (auto& sig : signatures) {
			code.append(Strings::Format("int Offsets::%s %s = %#010x;\n", sig.name, std::string(30 - strlen(sig.name), ' ').c_str(), sig.offset));
		}

		strcpy_s(CodeDump, code.c_str());
	}

	for (auto& sig : signatures) {
		ImGui::SetNextTreeNodeOpen(true);
		if (ImGui::TreeNode(sig.name)) {
			ImGui::TextColored(Color::YELLOW, sig.pattern);
			switch (sig.status) {

			case SCAN_NOT_STARTED:
				ImGui::TextColored(Color::GRAY, "Not scanned");
				break;
			case SCAN_IN_PROGRESS:
				ImGui::TextColored(Color::YELLOW, "Scanning");
				break;
			case SCAN_NOT_FOUND:
				ImGui::TextColored(Color::RED, "Not found");
				break;
			case SCAN_FOUND:
				ImGui::DragInt("Offset", &sig.offset, 1.f, 0, 0, "%#010x");
				break;
			}
			ImGui::TreePop();
		}
	}

	if (strlen(CodeDump) > 0) {
		ImGui::InputTextMultiline("Code Dump", CodeDump, 2048);
	}

	ImGui::End();
}

void OffsetScanner::Scan()
{
	auto module = GetModuleHandle(NULL);
	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)module + dosHeader->e_lfanew);
	auto textSection = IMAGE_FIRST_SECTION(ntHeaders);

	int start = (int)module + textSection->VirtualAddress;
	int size = textSection->SizeOfRawData;

	for (auto& sig : signatures) {
		sig.Scan(start, size);
	}
	Scanning = false;
}

OffsetSignature::OffsetSignature(const char* name, const char* pattern, int extractIndex, bool offsetIsBase)
{
	this->name = name;
	this->pattern = pattern;
	this->extractIndex = extractIndex;
	this->offsetIsAddress = offsetIsBase;

	std::string strByte;
	std::istringstream iss(std::string(pattern), std::istringstream::in);
	while (iss >> strByte) {
		if (strByte.compare("?") == 0) {
			bytes.push_back(0x00);
			mask.push_back(false);
		}
		else {
			bytes.push_back(std::stoi(strByte, 0, 16));
			mask.push_back(true);
		}
	}
}

void OffsetSignature::Scan(int startAddr, int size)
{
	status = SCAN_IN_PROGRESS;

	int endAddr = startAddr + size - bytes.size();
	auto memInfo = MEMORY_BASIC_INFORMATION{ 0 };

	int  pageStart = startAddr;
	int  pageEnd = startAddr;

	do {
		if (pageStart >= endAddr)
			break;

		if (!VirtualQuery((void*)pageStart, &memInfo, sizeof(MEMORY_BASIC_INFORMATION)))
			return;

		pageStart = (int)memInfo.BaseAddress;
		pageEnd = pageStart + memInfo.RegionSize;

		if (memInfo.Protect != PAGE_NOACCESS) {
			for (int addr = pageStart; addr < pageEnd - bytes.size(); ++addr) {

				char* mem = (char*)addr;
				bool matched = true;

				for (int i = 0; i < bytes.size(); ++i) {
					if (mask[i] && bytes[i] != mem[i]) {
						matched = false;
						break;
					}
				}

				if (matched) {
					if (offsetIsAddress) offset = (int)mem - (int)GetModuleHandle(NULL);
					else offset = *(int*)(mem + extractIndex) - (int)GetModuleHandle(NULL);
					status = SCAN_FOUND;
					return;
				}

			}
		}
		pageStart = pageEnd;

	} while (true);

	status = SCAN_NOT_FOUND;
}