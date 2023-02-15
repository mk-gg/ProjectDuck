#include "GameRenderer.h"
#include "Offset.h"
#include <cstring>

void GameRenderer::ReadFromBaseAddress(int baseAddr) 
{
	__try {
		memcpy(&viewMatrix, (void*)(baseAddr + Offset::ViewMatrix), 16 * sizeof(float));
		memcpy(&projMatrix, (void*)(baseAddr + Offset::ProjectionMatrix), 16 * sizeof(float));
	
		char* addrRenderer = (char*)*(int*)(baseAddr + Offset::Renderer);
		memcpy(&width, addrRenderer + Offset::RendererWidth, sizeof(int));
		memcpy(&height, addrRenderer + Offset::RendererHeight, sizeof(int));

		MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
	}
	__except (1) {}

	

	
}

void GameRenderer::MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2)
{
	int size = row1 * col2;
	for (int i = 0; i < row1; i++) {
		for (int j = 0; j < col2; j++) {
			float sum = 0.f;
			for (int k = 0; k < col1; k++)
				sum = sum + a[i * col1 + k] * b[k * col2 + j];
			out[i * col2 + j] = sum;
		}
	}
}

Vector2 GameRenderer::WorldToScreen(const Vector3& pos) const 
{

	Vector2 out = { 0.f, 0.f };
	Vector2 screen = { (float)width, (float)height };
	static Vector4 clipCoords;
	clipCoords.x = pos.x * viewProjMatrix[0] + pos.y * viewProjMatrix[4] + pos.z * viewProjMatrix[8] + viewProjMatrix[12];
	clipCoords.y = pos.x * viewProjMatrix[1] + pos.y * viewProjMatrix[5] + pos.z * viewProjMatrix[9] + viewProjMatrix[13];
	clipCoords.z = pos.x * viewProjMatrix[2] + pos.y * viewProjMatrix[6] + pos.z * viewProjMatrix[10] + viewProjMatrix[14];
	clipCoords.w = pos.x * viewProjMatrix[3] + pos.y * viewProjMatrix[7] + pos.z * viewProjMatrix[11] + viewProjMatrix[15];

	if (clipCoords.w < 1.0f)
		clipCoords.w = 1.f;

	Vector3 M;
	M.x = clipCoords.x / clipCoords.w;
	M.y = clipCoords.y / clipCoords.w;
	M.z = clipCoords.z / clipCoords.w;

	out.x = (screen.x / 2.f * M.x) + (M.x + screen.x / 2.f);
	out.y = -(screen.y / 2.f * M.y) + (M.y + screen.y / 2.f);


	return out;
}

Vector2 GameRenderer::WorldToMinimap(const Vector3& pos, const Vector2& wPos, const Vector2& wSize) const 
{

	Vector2 result = { pos.x / 15000.f, pos.z / 15000.f };
	result.x = wPos.x + result.x * wSize.x;
	result.y = wPos.y + wSize.y - (result.y * wSize.y);

	return result;
}

float GameRenderer::DistanceToMinimap(float dist, const Vector2& wSize) const 
{

	// This assumes that the minimap is a square !
	return (dist / 15000.f) * wSize.x;
}

bool GameRenderer::IsScreenPointOnScreen(const Vector2& point, float offsetX, float offsetY) const 
{
	return point.x > -offsetX && point.x < (width + offsetX) && point.y > -offsetY && point.y < (height + offsetY);
}

bool GameRenderer::IsWorldPointOnScreen(const Vector3& point, float offsetX, float offsetY) const 
{
	return IsScreenPointOnScreen(WorldToScreen(point), offsetX, offsetY);
}


void GameRenderer::DrawCircleAt(ImDrawList* canvas, const Vector3& worldPos, float radius, int numPoints, ImColor color, float thickness) const {

	if (numPoints >= 200)
		return;
	static Vector2 points[202];
	static Vector2 center;

	float step = 6.2831f / numPoints;
	float theta = 0.f;

	Vector2 screenSpace = WorldToScreen({ worldPos.x, worldPos.y, worldPos.z });
	center.x = screenSpace.x;
	center.y = screenSpace.y;

	screenSpace = WorldToScreen({ worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) });
	points[0].x = screenSpace.x;
	points[0].y = screenSpace.y;

	float thicknessFraction = thickness / 3.0f;
	for (int i = 1; i <= numPoints + 1; i++, theta += step) {
		screenSpace = WorldToScreen({ worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) });

		points[i].x = screenSpace.x;
		points[i].y = screenSpace.y;

		canvas->AddLine((ImVec2&)points[i - 1], (ImVec2&)points[i], color, thickness + thicknessFraction * sin(theta));
	}
}

void GameRenderer::DrawCircleAtFilled(ImDrawList* canvas, const Vector3& worldPos, float radius, int numPoints, ImColor color) const {

	if (numPoints >= 200)
		return;
	static ImVec2 points[200];

	float step = 6.2831f / numPoints;
	float theta = 0.f;
	for (int i = 0; i < numPoints; i++, theta += step) {
		Vector2 screenSpace = WorldToScreen({ worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) });

		points[i].x = screenSpace.x;
		points[i].y = screenSpace.y;
	}

	canvas->AddConvexPolyFilled(points, numPoints, color);
}