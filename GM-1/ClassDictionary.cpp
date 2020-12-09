#include "ClassDictionary.h"
#include <algorithm>
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "AABB.h"
#include "Billboard.h"
#include "Texture.h"
#include "Fbx.h"
#include "Animation.h"
#include "Plane.h"
#include "Item.h"
#include "Image.h"
#include "TitleText.h"
#include "TitleManager.h"
#include "Fade.h"
#include "MainGameManager.h"
#include "SkyDorm.h"
#include "WoodBox.h"
#include "ItemController.h"
#include "TutorialManager.h"
#include "ResultManager.h"
#include "Bullet.h"
#include "BoxCollider.h"
#include "CameraController.h"
#include "RigidBody.h"
#include "Spring.h"
#include "Morphing.h"

std::map<std::string, Component*> ClassDictionary::dictionary;
BuilderFromString ClassDictionary::builderfromstring;
std::vector<std::string> ClassDictionary::componentName;

void ClassDictionary::Initialize()
{
	builderfromstring.Register(REGISTER_ARGS(Camera));
	builderfromstring.Register(REGISTER_ARGS(Player));
	builderfromstring.Register(REGISTER_ARGS(Transform));
	builderfromstring.Register(REGISTER_ARGS(SphereCollider));
	builderfromstring.Register(REGISTER_ARGS(Billboard));
	builderfromstring.Register(REGISTER_ARGS(Texture));
	builderfromstring.Register(REGISTER_ARGS(Fbx));
	builderfromstring.Register(REGISTER_ARGS(Animation));
	builderfromstring.Register(REGISTER_ARGS(Plane));
	builderfromstring.Register(REGISTER_ARGS(AABB));
	builderfromstring.Register(REGISTER_ARGS(Item));
	builderfromstring.Register(REGISTER_ARGS(Image));
	builderfromstring.Register(REGISTER_ARGS(TitleText));
	builderfromstring.Register(REGISTER_ARGS(TitleManager));
	builderfromstring.Register(REGISTER_ARGS(Fade));
	builderfromstring.Register(REGISTER_ARGS(MainGameManager));
	builderfromstring.Register(REGISTER_ARGS(SkyDorm));
	builderfromstring.Register(REGISTER_ARGS(WoodBox));
	builderfromstring.Register(REGISTER_ARGS(ItemController));
	builderfromstring.Register(REGISTER_ARGS(TutorialManager));
	builderfromstring.Register(REGISTER_ARGS(ResultManager));
	builderfromstring.Register(REGISTER_ARGS(Bullet));
	builderfromstring.Register(REGISTER_ARGS(BoxCollider));
	builderfromstring.Register(REGISTER_ARGS(CameraController));
	builderfromstring.Register(REGISTER_ARGS(RigidBody));
	builderfromstring.Register(REGISTER_ARGS(Spring));
	builderfromstring.Register(REGISTER_ARGS(Morphing));
	componentName.push_back("Camera");
	componentName.push_back("Player");
	componentName.push_back("SphereCollider");
	componentName.push_back("Billboard");
	componentName.push_back("Texture");
	componentName.push_back("Fbx");
	componentName.push_back("Animation");
	componentName.push_back("Plane");
	componentName.push_back("AABB");
	componentName.push_back("Item");
	componentName.push_back("Image");
	componentName.push_back("TitleText");
	componentName.push_back("TitleManager");
	componentName.push_back("Fade");
	componentName.push_back("MainGameManager");
	componentName.push_back("SkyDorm");
	componentName.push_back("WoodBox");
	componentName.push_back("ItemController");
	componentName.push_back("TutorialManager");
	componentName.push_back("ResultManager");
	componentName.push_back("Bullet");
	componentName.push_back("BoxCollider");
	componentName.push_back("CameraController");
	componentName.push_back("RigidBody");
	componentName.push_back("Spring");
	componentName.push_back("Morphing");
	std::sort(componentName.begin(), componentName.end());
}

void ClassDictionary::Register(const std::string& ty_name, Component* component)
{
	dictionary.insert(std::make_pair(ty_name, component));
}

Component* ClassDictionary::AddComponent(const std::string& name, GameObject* obj)
{
	return obj->AddComponent(builderfromstring.Create(name));
}

Component* ClassDictionary::SetComponent(const std::string& name)
{
	Component* c = builderfromstring.Create(name);
	return c;
}

std::vector<std::string> ClassDictionary::GetComponentName()
{
	return componentName;
}