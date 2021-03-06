#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPS.h"
#include "imgui/imgui.h"
#include "Editor.h"
#include "Inspector.h"
#include "LevelLoader.h"

void RigidBody::SystemInitialize()
{
	m_force = { 0,0,0 };
	m_velocity = { 0,0,0 };
	m_oldForce = { 0,0,0 };
}

void RigidBody::FixedUpdate()
{
	if (!Editor::IsPlay()) return;
	m_force.y += m_gravity * FPS::deltaTime;
	m_force += m_impulseForce;
	Vector3 a = m_force / m_mass;
	m_velocity += a * FPS::deltaTime;
	gameObject->transform->position += m_velocity * FPS::deltaTime;
	m_impulseForce = { 0,0,0 };
}

void RigidBody::Finalize()
{
	m_force = { 0,0,0 };
	m_velocity = { 0,0,0 };
	m_oldForce = { 0,0,0 };
}

void RigidBody::SetGravity(float gravity)
{
	m_gravity = gravity;
}

float RigidBody::GetGrabity()
{
	return m_gravity;
}

void RigidBody::AddForce(Vector3 force, ForceMode forceMode)
{
	if (forceMode == ForceMode::FORCE)
	{
		m_force += force * FPS::deltaTime;
	}
	else if (forceMode == ForceMode::IMPULSE)
	{
		m_velocity += force;
	}
}

void RigidBody::SetForce(Vector3 force)
{
	m_force = force;
}

void RigidBody::SetVelocity(Vector3 velocity)
{
	m_velocity = velocity;
}

Vector3 RigidBody::GetForce()
{
	return m_force;
}

Vector3 RigidBody::GetVelocity()
{
	return m_velocity;
}

void RigidBody::SetKinematic(bool on)
{
	m_isKinematic = on;
}

bool RigidBody::IsKinematic()
{
	return m_isKinematic;
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
	ImGui::InputFloat("fx", &m_force.x, 1, 10);
	ImGui::InputFloat("fy", &m_force.y, 1, 10);
	ImGui::InputFloat("fz", &m_force.z, 1, 10);
	ImGui::InputFloat("vx", &m_velocity.x, 1, 10);
	ImGui::InputFloat("vy", &m_velocity.y, 1, 10);
	ImGui::InputFloat("vz", &m_velocity.z, 1, 10);
	ImGui::Checkbox("isKinematic", &m_isKinematic);

	if (ImGui::Button("Delete"))
	{
		this->SystemFinalize();
		gameObject->DeleteComponent<RigidBody>();
		Inspector::DeleteInformation();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void RigidBody::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetFloat(inProp, "mass", m_mass);
	JsonHelper::GetFloat(inProp, "gravity", m_gravity);
	JsonHelper::GetBool(inProp, "kinematic", m_isKinematic);
}

void RigidBody::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddFloat(alloc, inProp, "mass", m_mass);
	JsonHelper::AddFloat(alloc, inProp, "gravity", m_gravity);
	JsonHelper::AddBool(alloc, inProp, "kinematic", m_isKinematic);
}