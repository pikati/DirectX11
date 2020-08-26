#pragma once
#include "Component.h"

class AABB;
class Animation;

class Player : public Component
{
private:
    AABB* m_collider;
    Animation* m_animation;
    const float SPEED = 10.0f;
    const float m_gravity = 9.8f;
    Vector3 m_velocity;

    void Move();
    void Shot();
    void Jump();
public: 
    bool m_isGrounded = false;

    Player();
    ~Player();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};