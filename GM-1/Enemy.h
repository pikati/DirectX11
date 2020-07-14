#pragma once
#include "Component.h"

class CModel;
class SphereCollider;

class Enemy : public Component
{
private:
    CModel* m_model;
    SphereCollider* m_collider;
public:
    Enemy();
    ~Enemy();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};

