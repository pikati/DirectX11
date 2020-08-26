#include "LevelLoader.h"
#include "Scene.h"
#include "Engine.h"
#include "Component.h"
#include "Transform.h"
#include "ClassDictionary.h"
#include <fstream>
#include <ios>
#include <vector>
#include <memory>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
static Component* com;

bool LevelLoader::LoadLevel(Scene* scene, const char* fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		MessageBox(NULL, "failed to load level", NULL, MB_OK);
		return false;
	}

	const rapidjson::Value& objects = doc["GameObject"];
	if (objects.IsArray())
	{
		LoadGameObject(scene, objects);
	}
	return true;
}

bool LevelLoader::LoadJSON(const char* fileName, rapidjson::Document& outDoc)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		MessageBox(NULL, "not open JSON file", NULL, MB_OK);
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
		MessageBox(NULL, "not available JSON file", NULL, MB_OK);
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

			if (jObject.HasMember("properties"))
			{
				const rapidjson::Value& propertys = jObject["properties"];
				if (propertys.IsObject())
				{
					LoadProperties(obj, propertys);
				}
			}
			if (jObject.HasMember("component"))
			{
				const rapidjson::Value& components = jObject["component"];
				if (components.IsArray())
				{
					LoadComponent(obj, components);
				}
			}
			scene->AddGameObject(obj, false);
			std::list<Component*> comp = obj->GetComponents();
			for (Component* c : comp)
			{
				c->Initialize();
			}
		}	
	}
	
}

void LevelLoader::LoadProperties(GameObject* obj, const rapidjson::Value& inObj)
{
	JsonHelper::GetString(inObj, "name", obj->name);
	JsonHelper::GetString(inObj, "tag", obj->tag);
	JsonHelper::GetBool(inObj, "activeSelf", obj->activeSelf);
	JsonHelper::GetBool(inObj, "isDestroy", obj->isDestroy);
	JsonHelper::GetInt(inObj, "layer", obj->layer);
	JsonHelper::GetVector3(inObj, "position", obj->transform->position);
	JsonHelper::GetVector3(inObj, "rotation", obj->transform->rotation);
	JsonHelper::GetVector3(inObj, "scale", obj->transform->scale);
}

void LevelLoader::LoadComponent(GameObject* obj, const rapidjson::Value& inObj)
{
	int a = inObj.Size();
	for (rapidjson::SizeType i = 0; i < inObj.Size(); i++)
	{
		const rapidjson::Value& compObj = inObj[i];
		if (compObj.IsObject())
		{
			std::string type;
			if (JsonHelper::GetString(compObj, "type", type))
			{
				Component* component = ClassDictionary::AddComponent(type, obj);
				component->LoadProperties(compObj);
			}
		}
	}
}

void LevelLoader::SaveLevel(Scene* scene, const char* fileName)
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Value object(rapidjson::kArrayType);

	SaveGameObjects(doc.GetAllocator(), scene, object);
	doc.AddMember("GameObject", object, doc.GetAllocator());

	//JSONを文字列バッファに保存
	rapidjson::StringBuffer buffer;
	//成形出力用にPrettytWriterを使う
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();

	//output文字列をファイルに書く
	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}
}

void LevelLoader::SaveGameObjects(rapidjson::Document::AllocatorType& alloc, Scene* scene, 
	rapidjson::Value& inArray)
{
	const std::list<GameObject*>* objects = scene->GetAllGameObject();
	for (int i = 0; i < LAYER_MAX; i++)
	{
		for (GameObject* obj : objects[i])
		{
			//GameObject用のJSONオブジェクト
			rapidjson::Value jObj(rapidjson::kObjectType);
			//プロパティ用のJSOnオブジェクト
			rapidjson::Value props(rapidjson::kObjectType);
			SaveGameObject(alloc, obj, props);
			//プロパティをGameObjectのJSONオブジェクトに追加
			jObj.AddMember("properties", props, alloc);

			rapidjson::Value jComponents(rapidjson::kArrayType);
			SaveComponents(alloc, obj, jComponents);
			jObj.AddMember("component", jComponents, alloc);

			inArray.PushBack(jObj, alloc);
		}
	}
}

void LevelLoader::SaveGameObject(rapidjson::Document::AllocatorType& alloc, GameObject* obj, 
	rapidjson::Value& inObj)
{
	JsonHelper::AddString(alloc, inObj, "name", obj->name);
	JsonHelper::AddString(alloc, inObj, "tag", obj->tag);
	JsonHelper::AddBool(alloc, inObj, "activeSelf", obj->activeSelf);
	JsonHelper::AddBool(alloc, inObj, "isDestroy", obj->isDestroy);
	JsonHelper::AddInt(alloc, inObj, "layer", obj->layer);
	JsonHelper::AddVector3(alloc, inObj, "position", obj->transform->position);
	JsonHelper::AddVector3(alloc, inObj, "rotation", obj->transform->rotation);
	JsonHelper::AddVector3(alloc, inObj, "scale", obj->transform->scale);
}

void LevelLoader::SaveComponents(rapidjson::Document::AllocatorType& alloc, GameObject* obj, 
	rapidjson::Value& inArray)
{
	const std::list<Component*> components = obj->GetComponents();
	for (Component* comp : components)
	{
		rapidjson::Value jObj(rapidjson::kObjectType);
		comp->SaveProperties(alloc, jObj);
		inArray.PushBack(jObj, alloc);
	}
}

GameObject* LevelLoader::LoadPrefab(const char* fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		MessageBox(NULL, "failed to load level", NULL, MB_OK);
		return nullptr;
	}
	GameObject* obj = new GameObject();

	const rapidjson::Value& object = doc["GameObject"];
	if (object.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < object.Size(); i++)
		{
			const rapidjson::Value& jObject = object[i];
			if (jObject.IsObject())
			{

				if (jObject.HasMember("properties"))
				{
					const rapidjson::Value& propertys = jObject["properties"];
					if (propertys.IsObject())
					{
						LoadProperties(obj, propertys);
					}
				}
				if (jObject.HasMember("component"))
				{
					const rapidjson::Value& components = jObject["component"];
					if (components.IsArray())
					{
						LoadComponent(obj, components);
					}
				}
			}
		}
	}
	/*std::list<Component*> comp = obj->GetComponents();
	for (Component* com : comp)
	{
		com->Initialize();
	}*/
	return obj;
}

void LevelLoader::SavePrefab(GameObject* gameObject, const char* fileName)
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Value object(rapidjson::kArrayType);

	//GameObject用のJSONオブジェクト
	rapidjson::Value jObj(rapidjson::kObjectType);
	//プロパティ用のJSOnオブジェクト
	rapidjson::Value props(rapidjson::kObjectType);

	SaveGameObject(doc.GetAllocator(), gameObject, props);

	//プロパティをGameObjectのJSONオブジェクトに追加
	jObj.AddMember("properties", props, doc.GetAllocator());

	rapidjson::Value jComponents(rapidjson::kArrayType);
	SaveComponents(doc.GetAllocator(), gameObject, jComponents);
	jObj.AddMember("component", jComponents, doc.GetAllocator());

	object.PushBack(jObj, doc.GetAllocator());

	doc.AddMember("GameObject", object, doc.GetAllocator());

	//JSONを文字列バッファに保存
	rapidjson::StringBuffer buffer;
	//成形出力用にPrettytWriterを使う
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();

	//output文字列をファイルに書く
	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}
}

#pragma region Helper
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

	outVec3.x = (float)property[0].GetDouble();
	outVec3.y = (float)property[1].GetDouble();
	outVec3.z = (float)property[2].GetDouble();

	return true;
}

void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc, 
	rapidjson::Value& inObject, const char* name, int value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc, 
	rapidjson::Value& inObject, const char* name, float value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddString(rapidjson::Document::AllocatorType& alloc, 
	rapidjson::Value& inObject, const char* name, std::string value)
{
	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, bool value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddVector3(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const Vector3& value)
{
	// Create an array
	rapidjson::Value v(rapidjson::kArrayType);
	// Push back elements
	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.z).Move(), alloc);

	// Add array to inObject
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
#pragma endregion