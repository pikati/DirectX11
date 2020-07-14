#pragma once
#include <string>
#include <List>

class GameObject;

class SaveLoadManager
{
private:
	static void SetInfo(int count, std::string str, GameObject* obj);
public:
	static bool Save(const std::list<GameObject*> *obj);
	static std::list<GameObject*> Load(std::list<GameObject*> *obj);
	static GameObject* Instantiate(std::string path);
};

