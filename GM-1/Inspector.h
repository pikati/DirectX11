#pragma once
#include <vector>
class GameObject;

class Inspector
{
private:
	static GameObject* m_object;
	static std::vector<bool> m_isDrawInfo;
	static bool m_isComponent;
	static void DispObjectInformation();
	static void DispTransform();
	static void DispAddComponentWindow();
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	static void SetGameObject(GameObject* obj);
};

