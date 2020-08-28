#pragma once
#include "Component.h"

class AABB;
class Animation;

class Player : public Component
{
private:
    enum Direction
    {
        FORWARD,
        BACK,
        LEFT,
        RIGHT,
        NON
    };

    AABB* m_collider;
    Animation* m_animation;
    const float SPEED = 10.0f;
    const float m_gravity = 9.8f;
    Vector3 m_velocity;
    float m_attackFrame = 0;
    const float m_attackTime = 0.5f;
    bool m_isGrounded = false;
    Direction dir = NON;
    bool m_isAttack = false;

    void Move();
    void Shot();
    void Jump();
    void Slash();
    void CheckGrounded();

public: 

    Player();
    ~Player();
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    bool IsAttack();
};