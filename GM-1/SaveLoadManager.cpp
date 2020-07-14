#include "SaveLoadManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "ClassDictionary.h"
#include "Constants.h"

enum LoadInfo
{
	NAME,
	TAG,
	ACTIVE_SELF,
	DESTROY,
	LAYER,
	POS_X,
	POS_Y,
	POS_Z,
	END
};

bool SaveLoadManager::Save(const std::list<GameObject*> *obj)
{
	std::string filename = "test.txt";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* object : obj[i])
		{
			writing_file << "name: " << object->name << "\n";
			writing_file << "tag: " << object->tag << "\n";
			writing_file << "activeSelf: " << object->activeSelf << "\n";
			writing_file << "isDestroy: " << object->isDestroy << "\n";
			writing_file << "layer: " << i << "\n";
			writing_file << "x: " << object->transform->position.x << "\n";
			writing_file << "y: " << object->transform->position.y << "\n";
			writing_file << "z: " << object->transform->position.z << "\n";
			for (Component* component : object->GetComponents())
			{
				std::string componentName = typeid(*component).name();
				writing_file << componentName.substr(6) << std::endl;
			}
			writing_file << std::endl;
		}
	}
	
	return !writing_file.fail();
}

std::list<GameObject*> SaveLoadManager::Load(std::list<GameObject*> *obj)
{
	std::string filename = "test.txt";
	std::string readStr;
	std::ifstream reading_file;
	reading_file.open(filename, std::ios::in);
	GameObject* loadObj;
	while (true)
	{
		loadObj = new GameObject();
		int count = 0;
		while (std::getline(reading_file, readStr))
		{
			if (readStr == "") break;
			std::stringstream ss;
			ss << readStr;
			std::string getStr;
			while (!ss.eof())
			{
				ss >> getStr;
			}
			SetInfo(count++, getStr, loadObj);
		}
		if (reading_file.eof())
		{
			break;
		}
		obj[loadObj->layer].push_back(loadObj);
	}
	
	return *obj;
}

GameObject* SaveLoadManager::Instantiate(std::string path)
{
	std::string readStr;
	std::ifstream reading_file;
	reading_file.open(path, std::ios::in);
	GameObject* loadObj = new GameObject();
	int count = 0;
	while (std::getline(reading_file, readStr))
	{
		if (readStr == "") break;
		std::stringstream ss;
		ss << readStr;
		std::string getStr;
		while (!ss.eof())
		{
			ss >> getStr;
		}
		SetInfo(count++, getStr, loadObj);
	}
	return loadObj;
}

void SaveLoadManager::SetInfo(int count, std::string str, GameObject* obj)
{
	switch (count)
	{
	case NAME:
		obj->name = str;
		break;
	case TAG:
		obj->tag = str;
		break;
	case ACTIVE_SELF:
		obj->activeSelf = atoi(str.c_str());
		break;
	case DESTROY:
		obj->isDestroy = atoi(str.c_str());
		break;
	case LAYER:
		obj->layer = atoi(str.c_str());
		break;
	case POS_X:
		obj->transform->position.x = atof(str.c_str());
		break;
	case POS_Y:
		obj->transform->position.y = atof(str.c_str());
		break;
	case POS_Z:
		obj->transform->position.z = atof(str.c_str());
		break;
	default:
		ClassDictionary::AddComponent(str, obj);
		break;
	}

}