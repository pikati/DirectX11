#pragma once
#include <string>
#include "rapidjson/document.h"

class Scene;
class Vector3;

class LevelLoader
{
private:
	static bool LoadJSON(std::string& fileName, rapidjson::Document& outDoc);
	static void LoadGameObject(Scene* scene, const rapidjson::Value& inObj);
	static void LoadPropertys(GameObject* obj, const rapidjson::Value& inObj);
	static void LoadComponent(GameObject* obj, const rapidjson::Value& inObj);
public:
	static bool LoadLevel(Scene* scene, std::string& fileName);
};

class JsonHelper
{
public:
	static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVec3);
};
