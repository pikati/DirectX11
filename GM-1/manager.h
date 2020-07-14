#pragma once

class Scene;

class CManager
{
private:

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static Scene* GetScene();
};