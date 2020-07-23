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
#include "SpriteEffect.h"

std::map<std::string, Component*> ClassDictionary::dictionary;
BuilderFromString ClassDictionary::builderfromstring;
void ClassDictionary::Initialize()
{
	/*Register("Bullet", new Bullet());
	Register("Camera", new Camera());
	Register("Enemy", new Enemy());
	Register("Field", new Field());
	Register("Player", new Player());
	Register("Polygon", new CPolygon());*/
	builderfromstring.Register(REGISTER_ARGS(Bullet));
	builderfromstring.Register(REGISTER_ARGS(Camera));
	builderfromstring.Register(REGISTER_ARGS(Enemy));
	builderfromstring.Register(REGISTER_ARGS(Field));
	builderfromstring.Register(REGISTER_ARGS(Player));
	builderfromstring.Register(REGISTER_ARGS(CPolygon));
	builderfromstring.Register(REGISTER_ARGS(Transform));
	builderfromstring.Register(REGISTER_ARGS(SphereCollider));
	builderfromstring.Register(REGISTER_ARGS(SpriteEffect));
}

void ClassDictionary::Register(const std::string& ty_name, Component* component)
{
	dictionary.insert(std::make_pair(ty_name, component));
}

Component* ClassDictionary::AddComponent(const std::string& name, GameObject* obj)
{
	return obj->AddComponent(builderfromstring.Create(name));
}

Component* ClassDictionary::SetComponent(const std::string& name, Component* component)
{
	Component* c = builderfromstring.Create(name);
	component = c;
	return c;
}