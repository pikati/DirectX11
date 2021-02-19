#pragma once
#include <vector>
class GameObject;

struct ActiveObjInfo 
{
	GameObject* obj;
	int layer;
	int index;
};

class Inspector
{
private:
	static ActiveObjInfo* m_object;
	static std::vector<bool> m_isDrawInfo;
	static bool m_isComponent;
	static void DispObjectInformation();
	static void DispTransform();
	static void DispAddComponentWindow();
	static bool m_isDeleted;
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	static void DeleteObject();
	static void SetGameObject(GameObject* obj, int layer, int index);
	static void DeleteInformation();
	static void Copy();
};

