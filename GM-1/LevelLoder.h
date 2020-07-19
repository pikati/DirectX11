#pragma once
#include <string>
#include "rapidjson/document.h"

class Scene;

class LevelLoder
{
private:
	static bool LoadJSON(std::string& fileName, rapidjson::Document& outDoc);
public:
	static bool LoadLevel(Scene* scene, std::string& fileName);
};

class JsonHelper
{
public:
	static bool GetInt(const rapidjson::Value& inObject, const char* inPropaty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inPropaty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inPropaty, std::string& outString);
	static bool GetBool(const rapidjson::Value& inObject, const char* inPropaty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject, const char* inPropaty, Vector3& outVec3);
};
