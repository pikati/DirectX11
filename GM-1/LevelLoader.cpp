#include "LevelLoader.h"
#include "Scene.h"
#include "Engine.h"
#include <fstream>
#include <ios>
#include <vector>

bool LevelLoader::LoadLevel(Scene* scene, std::string& fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		assert("failed to load level %s", fileName.c_str());
		return false;
	}

	const rapidjson::Value& objects = doc["GameObjects"];
	if (objects.IsObject())
	{
		LoadGameObject(scene, objects);
	}
}

bool LevelLoader::LoadJSON(std::string& fileName, rapidjson::Document& outDoc)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		assert("not open %s.", fileName.c_str());
		return false;
	}

	//ファイルのサイズを取得
	std::ifstream::pos_type fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//末尾のヌルを含めたsize + 1 の配列を作る
	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	//配列にバイト列を読み込む
	file.read(bytes.data(), static_cast<size_t>(fileSize));

	outDoc.Parse(bytes.data());
	if (!outDoc.IsObject())
	{
		assert("not available JSON file %s.", fileName.c_str());
		return false;
	}
	return true;
}

void LevelLoader::LoadGameObject(Scene* scene, const rapidjson::Value& inObj)
{

	for (rapidjson::SizeType i = 0; i < inObj.Size(); i++)
	{
		const rapidjson::Value& jObject = inObj[i];
		if (jObject.IsObject())
		{
			GameObject* obj = new GameObject();
			scene->AddGameObject(obj);

			if (jObject.HasMember("Propertys"))
			{
				const rapidjson::Value& propertys = jObject["Propertys"];
				if (propertys.IsArray())
				{
					LoadPropertys(obj, inObj["Propertys"]);
				}
			}
			if (jObject.HasMember("Components"))
			{
				const rapidjson::Value& components = jObject["Components"];
				if (components.IsArray())
				{
					LoadComponent(obj, inObj["Component"]);
				}
			}
		}	
	}
	
}

void LevelLoader::LoadPropertys(GameObject* obj, const rapidjson::Value& inObj)
{
	JsonHelper::GetString(inObj, "name", obj->name);
	JsonHelper::GetString(inObj, "tag", obj->tag);
	JsonHelper::GetBool(inObj, "activeSelf", obj->activeSelf);
	JsonHelper::GetBool(inObj, "isDestroy", obj->isDestroy);
	JsonHelper::GetInt(inObj, "layer", obj->layer);

}

void LevelLoader::LoadComponent(GameObject* obj, const rapidjson::Value& inObj)
{

}








bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	//値の型を取得し、intであることを確認
	auto& propaty = itr->value;
	if (!propaty.IsInt())
	{
		return false;
	}

	outInt = propaty.GetInt();
	return true;
}

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& propaty = itr->value;
	if (!propaty.GetFloat())
	{
		return false;
	}

	outFloat = propaty.GetFloat();
	return true;
}

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& propaty = itr->value;
	if (!propaty.IsString())
	{
		return false;
	}

	outString = propaty.GetString();
	return true;
}

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& propaty = itr->value;
	if (!propaty.IsBool())
	{
		return false;
	}

	outBool = propaty.GetBool();
	return true;
}

bool JsonHelper::GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVec3)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsArray() || property.Size() != 3)
	{
		return false;
	}

	for (rapidjson::SizeType i = 0; i < 3; i++)
	{
		if (!property[i].IsDouble())
		{
			return false;
		}
	}

	outVec3.x = property[0].GetDouble();
	outVec3.y = property[1].GetDouble();
	outVec3.z = property[2].GetDouble();

	return true;
}