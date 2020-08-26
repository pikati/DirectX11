#pragma once
#include "Component.h"

class Fbx;
class SphereCollider;
class GameObject;
class Scene;

class Bullet : public Component
{
private:
    Fbx* m_model;
    SphereCollider* m_collider;
    int n = 0;
    Vector3 m_direction;
    Scene* m_scene;
    GameObject* m_player;
    GameObject* m_camera;
    GameObject* m_effect;
    int m_frame = 0;
    const float SPEED = 20.0f;
public:
    Bullet();
    ~Bullet();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};

