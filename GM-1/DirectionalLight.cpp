#include "DirectionalLight.h"
#include "imGui/imgui.h"
#include "LevelLoader.h"

void DirectionalLight::SystemInitialize()
{
	m_light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.Direction = { 1.0f, -1.0f, 1.0f, 0.0f };
	m_light.Enable = true;
	gameObject->layer = 0;
}

void DirectionalLight::SystemUpdate()
{
	m_light.Direction.Normalize();
	CRenderer::SetLight(m_light);
}

void DirectionalLight::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());
	float diffuse[4] = { m_light.Diffuse.x, m_light.Diffuse.y, m_light.Diffuse.z, m_light.Diffuse.w };
	float ambient[4] = { m_light.Ambient.x, m_light.Ambient.y, m_light.Ambient.z, m_light.Ambient.w };
	float direction[3] = { m_light.Direction.x, m_light.Direction.y, m_light.Direction.z};
	ImGui::ColorEdit4("diffuse", diffuse);
	ImGui::ColorEdit4("ambient", ambient);
	ImGui::InputFloat3("direction", direction, 3);
	m_light.Diffuse.x = diffuse[0];
	m_light.Diffuse.y = diffuse[1];
	m_light.Diffuse.z = diffuse[2];
	m_light.Diffuse.w = diffuse[3];
	m_light.Ambient.x = ambient[0];
	m_light.Ambient.y = ambient[1];
	m_light.Ambient.z = ambient[2];
	m_light.Ambient.w = ambient[3];
	m_light.Direction.x = direction[0];
	m_light.Direction.y = direction[1];
	m_light.Direction.z = direction[2];
	
	bool enable = true;
	if (m_light.Enable == 0)
	{
		enable = false;
	}
	
	ImGui::Checkbox("Enable", &enable);
	m_light.Enable = enable;
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void DirectionalLight::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetVector4(inProp, "diffuse", m_light.Diffuse);
	JsonHelper::GetVector4(inProp, "ambient", m_light.Ambient);
	JsonHelper::GetVector4(inProp, "direction", m_light.Direction);
	JsonHelper::GetInt(inProp, "enable", m_light.Enable);
	JsonHelper::GetInt(inProp, "id", m_id);
	//Initialize();
}

void DirectionalLight::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddVector4(alloc, inProp, "diffuse", m_light.Diffuse);
	JsonHelper::AddVector4(alloc, inProp, "ambient", m_light.Ambient);
	JsonHelper::AddVector4(alloc, inProp, "direction", m_light.Direction);
	JsonHelper::AddInt(alloc, inProp, "enable", m_light.Enable);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}