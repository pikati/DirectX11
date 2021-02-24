#pragma once
#include "Component.h"

class AABB;
class Animation;
class CameraController;
class ItemController;
class RigidBody;

class Cat : public Component
{
private:
    AABB* m_collider = nullptr;
    Animation* m_animation = nullptr;
    CameraController* m_camera = nullptr;
    ItemController* m_itemController = nullptr;
    RigidBody* m_rigidBody = nullptr;
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

    Cat();
    ~Cat();
    void Initialize() override;
    void Update() override;
    void OnCollisionEnter(GameObject* obj) override;
    void OnCollisionExit(GameObject* obj) override;
};

