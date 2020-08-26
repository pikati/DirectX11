#pragma once
#include "Component.h"

class AABB;
class Animation;

class Player : public Component
{
private:
    AABB* m_collider;
    Animation* m_animation;
    Vector3 m_velocity;
    const float SPEED = 10.0f;
    const float m_gravity = 9.8f;
    bool m_isGrounded = true;

    void Move();
    void Shot();
    void Jump();
    //D3DXQUATERNION m_quaternion;
public: 
    Player();
    ~Player();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};