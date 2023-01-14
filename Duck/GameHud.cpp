#include "GameHud.h"
#include "Offset.h"
#include "Memory.h"
#include <cstring>

void GameHud::ReadFromBaseAddress(int baseAddr)
{
	int minimapObject = ReadInt(baseAddr + Offset::MinimapObject);
	int minimapHud = ReadInt(minimapObject + Offset::MinimapObjectHud);

	memcpy(&minimapSize, AsPtr(minimapHud + Offset::MinimapHudSize), sizeof(Vector2));
	memcpy(&minimapPosition, AsPtr(minimapHud + Offset::MinimapHudPos), sizeof(Vector2));
}
