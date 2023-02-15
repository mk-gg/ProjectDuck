#pragma once
#include "MemoryReadable.h"
#include "Vector.h"
#include "SpellInfo.h"



class SpellCast : public MemoryReadable {

public:
	virtual void ReadFromBaseAddress(int address) override;
	void         ImGuiDraw();
	object       GetStaticData();
public:

	float       timeBegin;
	float       castTime;
	Vector3     start;
	Vector3     end;
	short       srcIndex;
	short       destIndex;

	std::string name;
	SpellInfo* staticData;
};
