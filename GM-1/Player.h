#pragma once
#include "Component.h"

class AABB;
class Animation;
class Camera;
class ItemController;

class Player : public Component
{
private:

    AABB* m_collider;
    Animation* m_animation;
    Camera* m_camera;
    ItemController* m_itemController;
    const float SPEED = 5.0f;
    const float m_gravity = 9.8f;
    Vector3 m_velocity;
    Vector3 m_direction = Vector3::zero;
    Vector3 m_startPosition = Vector3(-5.0f, 0.1f, 0.0f);
    bool m_isGrounded = false;

    void Move();
    void Shot();
    void Jump();

public: 

    Player();
    ~Player();
    void Initialize() override;
    void Update() override;
    void OnCollisionEnter(GameObject* obj) override;
    void OnCollisionExit(GameObject* obj) override;
};