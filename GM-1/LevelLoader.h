#pragma once
#include <string>
#include "rapidjson/document.h"

class Scene;
class Vector3;
class Vector4;
class GameObject;

class LevelLoader
{
private:
	static bool LoadJSON(const char* fileName, rapidjson::Document& outDoc);
	static void LoadGameObject(Scene* scene, const rapidjson::Value& inObj);
	static void LoadProperties(GameObject* obj, const rapidjson::Value& inObj);
	static void LoadComponent(GameObject* obj, const rapidjson::Value& inObj);
	static void SaveGameObjects(rapidjson::Document::AllocatorType& alloc, Scene* scenen, rapidjson::Value& inArray);
	static void SaveGameObject(rapidjson::Document::AllocatorType& alloc, GameObject* obj, rapidjson::Value& inObj);
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc, GameObject* obj, rapidjson::Value& inObj);
public:
	static bool LoadLevel(Scene* scene, const char* fileName);
	static void SaveLevel(Scene* scene, const char* fileName);
	static GameObject* LoadPrefab(const char* fileName);
	static void SavePrefab(GameObject* gameObject, const char* fileName);
	static bool ExistSceneFile(const std::string fileName);
	static bool ExistSceneFile(const char*  fileName);
};

class JsonHelper
{
public:
	static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVec3);
	static bool GetVector4(const rapidjson::Value& inObject, const char* inProperty, Vector4& outVec4);

	static void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value);
	static void AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, std::string value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value);
	static void AddVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector3& value);
	static void AddVector4(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector4& value);
};
