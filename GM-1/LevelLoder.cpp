#include "LevelLoder.h"
#include "Scene.h"
#include <fstream>
#include <ios>
#include <vector>

bool LevelLoder::LoadLevel(Scene* scene, std::string& fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		assert("failed to load level %s", fileName.c_str());
		return false;
	}
}

bool LevelLoder::LoadJSON(std::string& fileName, rapidjson::Document& outDoc)
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









bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inPropaty, int& outInt)
{
	auto itr = inObject.FindMember(inPropaty);
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

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inPropaty, float& outFloat)
{
	auto itr = inObject.FindMember(inPropaty);
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

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inPropaty, std::string& outString)
{
	auto itr = inObject.FindMember(inPropaty);
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

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inPropaty, bool& outBool)
{
	auto itr = inObject.FindMember(inPropaty);
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

bool JsonHelper::GetVector3(const rapidjson::Value& inObject, const char* inPropaty, Vector3& outVec3)
{
	rapidjson::SizeType num = inObject.Size();
	float vec[3];

	for (int i = 0; i < num; i++)
	{
		auto itr = inObject[i].FindMember(inPropaty);
		if (itr == inObject[i].MemberEnd())
		{
			return false;
		}

		auto& propaty = itr->value;
		if (!propaty.IsFloat())
		{
			return false;
		}
		vec[i] = propaty.GetFloat();
	}
	return true;
}