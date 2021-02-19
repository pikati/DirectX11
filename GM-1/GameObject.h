#pragma once
#include "Engine.h"
#include <string>
#include <list>
#include <typeinfo>
#include "rapidjson/document.h"
#include "imgui/ImGuizmo.h"
#include "main.h"

class Component;
class Transform;
class Camera;
class BoundingBox;

class GameObject
{
private:
	std::list<Component*> components;
	bool m_isDraw = true;
	bool m_isDrawBB = false;
	BoundingBox* m_bb = nullptr;
	int m_drawSortNum = 1;
	bool m_isActiveGizmo = false;
	Camera* m_camera;
	ImGuizmo::OPERATION m_currentOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE m_currentMode = ImGuizmo::LOCAL;
	bool m_isSnap = false;
	Transform* m_initialTransform;

	bool DeleteComponent(Component* component);
	void ImGuizmoUpdate();
	void SetGizmoMatrix(float matrix[16], const D3DXMATRIX& mat);
	void CalcMatrix(D3DXMATRIX& mat);
protected:
public:
	GameObject(const GameObject& obj);
	std::string name = "unname";
	std::string tag = "untagged";
	bool activeSelf = true;
	bool isDestroy = false;
	Transform* transform;
	int layer = 1;

	GameObject();
	~GameObject();
	void Initialize();
	void SystemInitialize();
	void Update();
	void FixedUpdate();
	void SystemUpdate();
	void Draw();
	void Finalize();
	void SystemFinalize();

	void Destroy();
	void OnCollisionEnter(GameObject* obj);
	void OnCollisionExit(GameObject* obj);

	Vector3 GetForward();
	Vector3 GetRight();
	Vector3 GetUp();

	bool DeleteComponent(std::string className);

	void IsDraw(bool isDraw);
	void SetBoundingBox(BoundingBox* bb);
	BoundingBox* GetBoundingBox();

	std::list<Component*> GetComponents() const;
	void SetComponents(std::list<Component*> component);

	void IsBoundingBoxDraw(bool isDraw);
	void SetSortingNum(int sortingNum);

	void SetActiveGizmo(bool active);



	template <typename T>
	T* AddComponent()
	{
		T* component = new T();
		components.push_back(component);
		component->SetGameObject(this);
		return component;
	}

	Component* AddComponent(Component* component);

	template <typename T>
	T* GetComponent()
	{
		T* component = nullptr;
		for (Component* c : components)
		{
			if (typeid(*c) == typeid(T))
			{
				component = (T*)c;
			}
		}
		return component;
	}

	Component* GetComponent(std::string name);

	template<typename T>
	void DeleteComponent()
	{
		for (Component* component : components)
		{
			if (typeid(*component) == typeid(T))
			{
				components.remove(component);
				DeleteComponent(component);
				return;
			}
		}
	}
};