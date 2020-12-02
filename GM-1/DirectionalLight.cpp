#include "DirectionalLight.h"
#include "imGui/imgui.h"

void DirectionalLight::Initialize()
{
	m_light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.Direction = { 1.0f, -1.0f, 1.0f, 0.0f };
	m_light.Enable = true;
}

void DirectionalLight::Update()
{
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
	int dr = (int)(m_light.Diffuse.r * 255);
	int dg = (int)(m_light.Diffuse.g * 255);
	int db = (int)(m_light.Diffuse.b * 255);
	int da = (int)(m_light.Diffuse.a * 255);
	ImGui::Text("Diffuse");
	ImGui::InputInt("R", &dr, 0, 0); ImGui::SameLine();
	ImGui::InputInt("G", &dg, 0, 0); ImGui::SameLine();
	ImGui::InputInt("B", &db, 0, 0); ImGui::SameLine();
	ImGui::InputInt("A", &da, 0, 0);
	int ar = (int)(m_light.Diffuse.r * 255);
	int ag = (int)(m_light.Diffuse.g * 255);
	int ab = (int)(m_light.Diffuse.b * 255);
	int aa = (int)(m_light.Diffuse.a * 255);
	ImGui::Text("Ambient");
	ImGui::InputInt("R ", &ar, 0, 0); ImGui::SameLine();
	ImGui::InputInt("G ", &ag, 0, 0); ImGui::SameLine();
	ImGui::InputInt("B ", &ab, 0, 0); ImGui::SameLine();
	ImGui::InputInt("A ", &aa, 0, 0);
	m_light.Diffuse.r = dr / 255.0f;
	m_light.Diffuse.g = dg / 255.0f;
	m_light.Diffuse.b = db / 255.0f;
	m_light.Diffuse.a = da / 255.0f;
	m_light.Ambient.r = ar / 255.0f;
	m_light.Ambient.g = ag / 255.0f;
	m_light.Ambient.b = ab / 255.0f;
	m_light.Ambient.a = aa / 255.0f;
	
	ImGui::Checkbox("Enable", (bool*)m_light.Enable);
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}