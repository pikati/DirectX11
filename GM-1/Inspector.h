#pragma once

class GameObject;

class Inspector
{
private:
	static GameObject* m_object;
	static void DispObjectInformation();

public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	static void SetGameObject(GameObject* obj);
};

