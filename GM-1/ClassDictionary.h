//•¶š—ñ‚©‚çComponent‚ğ“®“I‚É¶¬‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX
#pragma once
#include <map>
#include <vector>
#include <string>
#include "Component.h"
#define REGISTER_ARGS(TYPE) #TYPE,new ObjectBuilder<TYPE>

class IObjectBuilder
{
public:
	virtual Component* Create() = 0;
};

template<typename T>
class ObjectBuilder : public IObjectBuilder
{
public:
	virtual T* Create()
	{
		return new T;
	}
};

class BuilderFromString
{
	std::map<std::string, IObjectBuilder*> builders;
public:

	void Register(const std::string& ty_name, IObjectBuilder* builder)
	{
		builders.insert(std::make_pair(ty_name, builder));
	}

	Component* Create(const std::string& ty_name)
	{
		std::map<std::string, IObjectBuilder*>::iterator itr = builders.find(ty_name);
		if (itr == builders.end())return 0;
		return (*itr).second->Create();
	}
};

class ClassDictionary
{
private:
	static std::map<std::string, Component*> dictionary;
	static BuilderFromString builderfromstring;
	static std::vector<std::string> componentName;
public:
	static void Initialize();
	static void Register(const std::string& ty_name, Component* component);
	static Component* AddComponent(const std::string& name, GameObject* obj);
	static Component* SetComponent(const std::string& name);
	static std::vector<std::string> GetComponentName();
};