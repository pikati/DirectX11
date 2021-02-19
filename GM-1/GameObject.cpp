#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "ClassDictionary.h"
#include <sstream>
#include "main.h"
#include <DirectXMath.h>
#include "LevelLoader.h"
#include "BoundingBox.h"
#include "imgui/imgui.h"
#include "input.h"
#include "manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Editor.h"

GameObject::GameObject()
{
	transform = new Transform();
	m_camera = nullptr;
}

GameObject::GameObject(const GameObject& obj)
{
	transform = new Transform(*obj.transform);
	std::list<Component*> c2;
	this->name = obj.name;
	this->tag = obj.tag;
	this->activeSelf = obj.activeSelf;
	this->isDestroy = obj.isDestroy;
	this->layer = obj.layer;
	for (Component* component : obj.components)
	{
		std::string typeName = typeid(*component).name();
		std::stringstream ss;
		ss << typeName;
		std::string getStr;
		while (!ss.eof())
		{
			ss >> getStr;
		}
		Component* c = ClassDictionary::SetComponent(getStr);
		c->SetProperties(component);
		c2.push_back(c);
		c->SetGameObject(this);
		c->SetID(component->GetID());
		c->SystemInitialize();
	}
	this->components = c2;
}

GameObject::~GameObject()
{

}

void GameObject::Initialize()
{
	transform->Initialize();
	//transform->SetGameObject(this);
	for (Component* c : components)
	{
		c->Initialize();
		//c->SetGameObject(this);
	}
	Scene* s = CManager::GetScene();
	GameObject* o = s->Find<Camera>();

	m_camera = o->GetComponent<Camera>();
}

void GameObject::SystemInitialize()
{
	transform->SystemInitialize();
	transform->SetGameObject(this);
	for (Component* c : components)
	{
		c->SystemInitialize();
		c->SetGameObject(this);
	}
	m_initialTransform = new Transform(*transform);
}

void GameObject::Update()
{
	if (!activeSelf) return;
	m_isDraw = true;
	transform->Update();
	for (Component* c : components)
	{
		c->Update();
	}
}

void GameObject::FixedUpdate()
{
	if (!activeSelf) return;
	for (Component* c : components)
	{
		c->FixedUpdate();
	}
}

void GameObject::SystemUpdate()
{
	transform->SystemUpdate();
	ImGuizmoUpdate();
	for (Component* c : components)
	{
		c->SystemUpdate();
	}
}

void GameObject::Draw()
{
	if (!activeSelf) return;
	if (!m_isDraw) return;

	transform->Draw();
	for (Component* c : components)
	{
		c->Draw();
	}
	if (m_isDrawBB)
	{
		m_bb->Draw(transform);
	}
}

void GameObject::Finalize()
{
	for (Component* c : components)
	{
		c->Finalize();
	}
	m_camera = nullptr;
	SAFE_DELETE(transform);
	transform = new Transform(*m_initialTransform);
	SAFE_DELETE(m_initialTransform);
}

void GameObject::SystemFinalize()
{
	transform->Finalize();
	SAFE_DELETE(transform);
	SAFE_DELETE(m_initialTransform);
	for (Component* c : components)
	{
		c->SystemFinalize();
	}
}

void GameObject::OnCollisionEnter(GameObject* obj)
{
	for (Component* c : components)
	{
		c->OnCollisionEnter(obj);
	}
}

void GameObject::OnCollisionExit(GameObject* obj)
{
	for (Component* c : components)
	{
		c->OnCollisionExit(obj);
	}
}

Vector3 GameObject::GetForward()
{
	using namespace DirectX;
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, XMConvertToRadians(transform->rotation.y), XMConvertToRadians(transform->rotation.x), XMConvertToRadians(transform->rotation.z));
	Vector3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;
	forward.Normalize();
	return forward;
}

Vector3 GameObject::GetRight()
{
	using namespace DirectX;
	D3DXMATRIX rot;
	float roty = transform->rotation.y + 90;
	D3DXMatrixRotationYawPitchRoll(&rot, XMConvertToRadians(roty), XMConvertToRadians(transform->rotation.x), XMConvertToRadians(transform->rotation.z));
	Vector3 right;
	right.x = rot._31;
	right.y = rot._32;
	right.z = rot._33;
	right.Normalize();
	return right;
}
Vector3 GameObject::GetUp()
{
	return Vector3::Normalize(Vector3::Cross(GetForward(), GetRight()));
}

bool GameObject::DeleteComponent(std::string className)
{
	for (Component* c : components)
	{
		std::string name = typeid(*c).name();
		if (name.substr(6) == className)
		{
			components.remove(c);
			DeleteComponent(c);
			return true;
		}
	}
	
	return false;
}

BoundingBox* GameObject::GetBoundingBox()
{
	if (m_bb != nullptr)
	{
		return m_bb;
	}
	return nullptr;
}

void GameObject::SetBoundingBox(BoundingBox* bb)
{
	m_bb = bb;
}

void GameObject::IsDraw(bool isDraw)
{
	m_isDraw = isDraw;
}

Component* GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->SetGameObject(this);
	return component;
}

Component* GameObject::GetComponent(std::string name)
{
	name = "class " + name;
	for (Component* c : components)
	{
		if (typeid(*c).name() == name)
		{
			return c;
		}
	}
	return nullptr;
}

void GameObject::Destroy()
{
	isDestroy = true;
}

std::list<Component*> GameObject::GetComponents() const
{
	return components;
}

void GameObject::SetComponents(std::list<Component*> components)
{
	this->components = components;
}

bool GameObject::DeleteComponent(Component* component)
{
	component->Finalize();
	delete component;
	return true;
}

void GameObject::IsBoundingBoxDraw(bool isDraw)
{
	m_isDrawBB = isDraw;
}

void GameObject::SetSortingNum(int sortingNum)
{
	m_drawSortNum = sortingNum;
}

void GameObject::ImGuizmoUpdate()
{
	if (Editor::IsPlay()) return;
	//if (m_isActiveGizmo)
	//{
	//	float snap[3] = { 1.f, 1.f, 1.f };
	//	float matrix[16];
	//	float cameraView[16];
	//	float cameraProjection[16];
	//	float identityMatrix[16];
	//	D3DXMATRIX mat;
	//	D3DXMATRIX vmat = Editor::GetEditorCameraViewMatrix();
	//	D3DXMATRIX wmat = Editor::GetEditorCameraProjectionMatrix();
	//	D3DXMATRIX imat;
	//	D3DXMatrixIdentity(&imat);
	//	CalcMatrix(mat);
	//	SetGizmoMatrix(matrix, mat);
	//	SetGizmoMatrix(cameraView, vmat);
	//	SetGizmoMatrix(cameraProjection, wmat);
	//	SetGizmoMatrix(identityMatrix, imat);
	//	ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	//	ImGuizmo::SetID(0);
	//	ImGui::Begin("GizmoInfo");
	//	//if (CInput::GetKeyTrigger('W'))
	//	//	m_currentOperation = ImGuizmo::TRANSLATE;
	//	//if (CInput::GetKeyTrigger('E'))
	//	//	m_currentOperation = ImGuizmo::ROTATE;
	//	//if (CInput::GetKeyTrigger('R')) // r Key
	//	//	m_currentOperation = ImGuizmo::SCALE;
	//	if (ImGui::RadioButton("Translate", m_currentOperation == ImGuizmo::TRANSLATE))
	//		m_currentOperation = ImGuizmo::TRANSLATE;
	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("Rotate", m_currentOperation == ImGuizmo::ROTATE))
	//		m_currentOperation = ImGuizmo::ROTATE;
	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("Scale", m_currentOperation == ImGuizmo::SCALE))
	//		m_currentOperation = ImGuizmo::SCALE;
	//	float matT[3], matR[3], matS[3];
	//	ImGuizmo::DecomposeMatrixToComponents(matrix, matT, matR, matS);
	//	ImGui::InputFloat3("Tr", matT);
	//	ImGui::InputFloat3("Rt", matR);
	//	ImGui::InputFloat3("Sc", matS);
	//	ImGuizmo::RecomposeMatrixFromComponents(matT, matR, matS, matrix);
	//	transform->position.Set(matT[0], matT[1], matT[2]);
	//	transform->rotation.Set(matR[0], matR[1], matR[2]);
	//	transform->scale.Set(matS[0], matS[1], matS[2]);

	//	if (m_currentOperation != ImGuizmo::SCALE)
	//	{
	//		if (ImGui::RadioButton("Local", m_currentMode == ImGuizmo::LOCAL))
	//			m_currentMode = ImGuizmo::LOCAL;
	//		ImGui::SameLine();
	//		if (ImGui::RadioButton("World", m_currentMode == ImGuizmo::WORLD))
	//			m_currentMode = ImGuizmo::WORLD;
	//	}
	//	/*if (CInput::GetKeyTrigger('S'))
	//		m_isSnap = !m_isSnap;*/
	//	ImGuiIO& io = ImGui::GetIO();
	//	float camDistance = Editor::GetEditorCameraDistance();
	//	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//	ImGuizmo::Manipulate(cameraView, cameraProjection, m_currentOperation, m_currentMode, matrix, NULL, m_isSnap ? &snap[0] : NULL);
	//	ImGui::End();
	//}
}

void GameObject::SetActiveGizmo(bool active)
{
	m_isActiveGizmo = active;
}

void GameObject::SetGizmoMatrix(float m[16], const D3DXMATRIX& mat )
{
	/*m[0] = mat._11;
	m[4] = mat._12;
	m[8] = mat._13;
	m[1] = mat._21;
	m[5] = mat._22;
	m[9] = mat._23;
	m[2] = mat._31;
	m[6] = mat._32;
	m[10] = mat._33;
	m[3] = mat._41;
	m[7] = mat._42;
	m[11] = mat._43;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;*/
	m[0] = mat._11;
	m[1] = mat._12;
	m[2] = mat._13;
	m[3] = mat._14;
	m[4] = mat._21;
	m[5] = mat._22;
	m[6] = mat._23;
	m[7] = mat._24;
	m[8] = mat._31;
	m[9] = mat._32;
	m[10] = mat._33;
	m[11] = mat._34;
	m[12] = mat._41;
	m[13] = mat._42;
	m[14] = mat._43;
	m[15] = mat._44;
}

void GameObject::CalcMatrix(D3DXMATRIX& mat)
{
	using namespace DirectX;
	D3DXMATRIX world, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(transform->rotation.y), XMConvertToRadians(transform->rotation.x), XMConvertToRadians(transform->rotation.z));
	world._11 = transform->scale.x * rotation._11;
	world._12 = transform->scale.x * rotation._12;
	world._13 = transform->scale.x * rotation._13;
	world._21 = transform->scale.y * rotation._21;
	world._22 = transform->scale.y * rotation._22;
	world._23 = transform->scale.y * rotation._23;
	world._31 = transform->scale.z * rotation._31;
	world._32 = transform->scale.z * rotation._32;
	world._33 = transform->scale.z * rotation._33;
	world._41 = transform->position.x;
	world._42 = transform->position.y;
	world._43 = transform->position.z;
	world._14 = 0;
	world._24 = 0;
	world._34 = 0;
	world._44 = 1.0f;
	mat = world;
}