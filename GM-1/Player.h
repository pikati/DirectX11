#pragma once
#include "Component.h"

class CModel;
class SphereCollider;

class Player : public Component
{
private:
    CModel*     m_model;
    SphereCollider* m_collider;
    //D3DXQUATERNION m_quaternion;
public: 
    Player();
    ~Player();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};