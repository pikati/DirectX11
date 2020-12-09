#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPS.h"
#include "imgui/imgui.h"
#include "Editor.h"

void RigidBody::Initialize()
{
	m_force = { 0,0,0 };
	m_velocity = { 0,0,0 };
	m_oldForce = { 0,0,0 };
}

void RigidBody::Update()
{
	
}

void RigidBody::Draw()
{
	if (!Editor::IsPlay()) return;
	m_force.y = m_gravity;
	m_force += m_impulseForce;
	Vector3 a = m_force / m_mass;
	m_velocity += a * FPS::deltaTime;
	gameObject->transform->position += m_velocity * FPS::deltaTime;
	m_impulseForce = { 0,0,0 };
}

void RigidBody::SetGravity(float gravity)
{
	m_gravity = gravity;
}

float RigidBody::GetGrabity()
{
	return m_gravity;
}

void RigidBody::AddForce(Vector3 force, FORCE_MODE forceMode)
{
	if (forceMode == FORCE_MODE::FORCE)
	{
		m_force += force;
	}
	else if (forceMode == FORCE_MODE::IMPULSE)
	{
		m_velocity += force;
	}
}

void RigidBody::SetForce(Vector3 force)
{
	m_force = force;
}

Vector3 RigidBody::GetForce()
{
	return m_force;
}

Vector3 RigidBody::GetVelocity()
{
	return m_velocity;
}

void RigidBody::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	ImGui::InputFloat("mass", &m_mass, 1, 10);
	ImGui::InputFloat("gravity", &m_gravity, 1, 10);
	ImGui::InputFloat("x", &m_velocity.x, 1, 10);
	ImGui::InputFloat("y", &m_velocity.y, 1, 10);
	ImGui::InputFloat("z", &m_velocity.z, 1, 10);
	
	
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}