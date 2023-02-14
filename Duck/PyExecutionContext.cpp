#include "PyExecutionContext.h"
#include "Script.h"

template <class T>
std::shared_ptr<list> MakePyList(std::vector<std::shared_ptr<T>>& cList) {

	std::shared_ptr<list> pyList = std::shared_ptr<list>(new list());

	for (auto& v : cList) {
		pyList->append(v.get());
	}

	return pyList;
}

void PyExecutionContext::Log(const char* msg)
{
	if (msg == NULL)
		Logger::Console.Log("NULL");
	else
		Logger::Console.Log(msg);

}
Vector2 PyExecutionContext::World2Screen(const Vector3& world)
{
	return state->renderer.WorldToScreen(world);
}

Vector2 PyExecutionContext::World2Minimap(const Vector3& world)
{
	return state->renderer.WorldToMinimap(world, state->hud.minimapPosition, state->hud.minimapSize);
}

float PyExecutionContext::DistanceOnMinimap(float dist)
{
	return state->renderer.DistanceToMinimap(dist, state->hud.minimapSize);
}

bool PyExecutionContext::IsScreenPointOnScreen(const Vector2& point, float offsetX, float offsetY)
{
	return state->renderer.IsScreenPointOnScreen(point, offsetX, offsetY);
}

bool PyExecutionContext::IsWorldPointOnScreen(const Vector3& point, float offsetX, float offsetY)
{
	return state->renderer.IsWorldPointOnScreen(point, offsetX, offsetY);
}

object PyExecutionContext::GetImGuiInterface()
{
	return object(boost::ref(imgui));
}

object PyExecutionContext::GetChampions()
{
	return object(boost::ref(*champs));
}

object PyExecutionContext::GetMissiles()
{
	return object(boost::ref(*missiles));
}

object PyExecutionContext::GetJungle()
{
	return object(boost::ref(*jungle));
}

object PyExecutionContext::GetMinions()
{
	return object(boost::ref(*minions));
}

object PyExecutionContext::GetTurrets()
{
	return object(boost::ref(*turrets));
}

object PyExecutionContext::GetOthers()
{
	return object(boost::ref(*others));
}

object PyExecutionContext::GetConfig()
{
	return object(boost::ref(currentScript->config));
}

void PyExecutionContext::SetScript(Script* script)
{
	this->currentScript = script;
}



void PyExecutionContext::SetGameState(GameState* state)
{
	this->state = state;
	time = state->time;

	hovered = object(ptr(state->hovered.get()));
	player = object(ptr(state->player.get()));

	champs = MakePyList(state->champions);
	minions = MakePyList(state->minions);
	turrets = MakePyList(state->turrets);
	jungle = MakePyList(state->jungle);
	missiles = MakePyList(state->missiles);
	others = MakePyList(state->others);
}

void PyExecutionContext::SetImGuiOverlay(ImDrawList* overlay)
{
	this->overlay = overlay;
}

void PyExecutionContext::DrawRectWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, float thickness, const ImVec4& color)
{
	static Vector2 points[4];
	points[0] = state->renderer.WorldToScreen(p1);
	points[1] = state->renderer.WorldToScreen(p2);
	points[2] = state->renderer.WorldToScreen(p3);
	points[3] = state->renderer.WorldToScreen(p4);

	overlay->AddPolyline((ImVec2*)points, 4, ImColor(color), true, thickness);
}

void PyExecutionContext::DrawTriangleWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3, float thickness, const ImVec4& color)
{
	overlay->AddTriangle(
		(ImVec2&)state->renderer.WorldToScreen(p1),
		(ImVec2&)state->renderer.WorldToScreen(p2),
		(ImVec2&)state->renderer.WorldToScreen(p3), ImColor(color));
}

void PyExecutionContext::DrawTriangleWorldFilled(const Vector3& p1, const Vector3& p2, const Vector3& p3, const ImVec4& color)
{
	overlay->AddTriangleFilled(
		(ImVec2&)state->renderer.WorldToScreen(p1),
		(ImVec2&)state->renderer.WorldToScreen(p2),
		(ImVec2&)state->renderer.WorldToScreen(p3), ImColor(color));
}

void PyExecutionContext::DrawCircle(const Vector2& center, float radius, int numPoints, float thickness, const ImVec4& color)
{
	overlay->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color), numPoints, thickness);
}

void PyExecutionContext::DrawCircleFilled(const Vector2& center, float radius, int numPoints, const ImVec4& color)
{
	overlay->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color), numPoints);
}

void PyExecutionContext::DrawCircleWorld(const Vector3& center, float radius, int numPoints, float thickness, const ImVec4& color)
{
	state->renderer.DrawCircleAt(overlay, center, radius, numPoints, ImColor(color), thickness);
}

void PyExecutionContext::DrawCircleWorldFilled(const Vector3& center, float radius, int numPoints, const ImVec4& color)
{
	state->renderer.DrawCircleAtFilled(overlay, center, radius, numPoints, ImColor(color));
}

void PyExecutionContext::DrawLine(const Vector2& start, const Vector2& end, float thickness, const ImVec4& color)
{
	overlay->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
}

void PyExecutionContext::DrawImage(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color)
{
	static ImVec2 zero = ImVec2(0.f, 0.f);
	static ImVec2 one = ImVec2(1.f, 1.f);

	std::string imageName(img);
	overlay->AddImage(GameData::GetImage(imageName), (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color));
}

void PyExecutionContext::DrawImageRounded(const char* img, const Vector2& start, const Vector2& end, const ImVec4& color, float rounding)
{
	static ImVec2 zero = ImVec2(0.f, 0.f);
	static ImVec2 one = ImVec2(1.f, 1.f);

	std::string imageName(img);
	overlay->AddImageRounded(GameData::GetImage(imageName), (ImVec2&)start, (ImVec2&)end, zero, one, ImColor(color), rounding);
}

void PyExecutionContext::DrawTxt(const Vector2& pos, const char* text, const ImVec4& color)
{
	overlay->AddText(ImVec2(pos.x, pos.y), ImColor(color), text);
}

void PyExecutionContext::DrawRect(const Vector4& box, const ImVec4& color, float rounding, float thickness)
{
	overlay->AddRect(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding, 15, thickness);
}

void PyExecutionContext::DrawRectFilled(const Vector4& box, const ImVec4& color, float rounding)
{
	overlay->AddRectFilled(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color), rounding);
}