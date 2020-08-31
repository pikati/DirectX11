#include "ClassDictionary.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Camera.h"
#include "Enemy.h"
#include "Field.h"
#include "Player.h"
#include "Polygon.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "AABB.h"
#include "SpriteEffect.h"
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

std::map<std::string, Component*> ClassDictionary::dictionary;
BuilderFromString ClassDictionary::builderfromstring;

void ClassDictionary::Initialize()
{
	builderfromstring.Register(REGISTER_ARGS(Bullet));
	builderfromstring.Register(REGISTER_ARGS(Camera));
	builderfromstring.Register(REGISTER_ARGS(Enemy));
	builderfromstring.Register(REGISTER_ARGS(Field));
	builderfromstring.Register(REGISTER_ARGS(Player));
	builderfromstring.Register(REGISTER_ARGS(CPolygon));
	builderfromstring.Register(REGISTER_ARGS(Transform));
	builderfromstring.Register(REGISTER_ARGS(SphereCollider));
	builderfromstring.Register(REGISTER_ARGS(SpriteEffect));
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