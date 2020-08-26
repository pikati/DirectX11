#pragma once
#include "Component.h"

class CModel;
class AABB;

class Enemy : public Component
{
private:
    CModel* m_model;
    AABB* m_collider;
public:
    Enemy();
    ~Enemy();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};

