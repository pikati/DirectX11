#pragma once
#include "Component.h"

class CModel;
class SphereCollider;

class Bullet : public Component
{
private:
    CModel* m_model;
    SphereCollider* m_collider;
    int n = 0;
public:
    Bullet();
    ~Bullet();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};

