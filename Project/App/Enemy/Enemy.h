#pragma once

#include "BaseEnemy.h"
#include <Systems/JsonBinder/JsonBinder.h>
#include <App/CircleShadow/CircleShadow.h>
#include <memory>
class Enemy : public BaseEnemy
{
public:
    Enemy();
    ~Enemy() = default;

    void Initialize() override;
    void Update() override;
    void Draw(const Camera* _camera) override;

    void SetPosition(const Vector3& _pos) { model_->translate_ = _pos; }

    void ImGui() override;

    void OnCollision(const Collider* _other);

    void SetPlayerPosition(const WorldTransform* _playerWT) { playerWT_ = _playerWT; }

#ifdef _DEBUG
    void SetIdleFlag(bool _flag) { idleFlag_ = _flag; }
#endif // _DEBUG


private:
    void InitCollider();

    void Idle();

    void ChasePlayer();

    void Rotation();

    static uint32_t enemyCount_;
    uint32_t enemyID_;

    const WorldTransform* playerWT_ = nullptr;

    std::function<void()> f_currentState_;

    float chaseEndDistance_ = 1.0f;
    float moveSpeed_ = 0.1f;
    float targetAngle_ = 0.0f;

    std::unique_ptr<CircleShadow> circleShadow_ = nullptr;

    std::unique_ptr<JsonBinder> jsonBinder_;
    std::string modelPath_ = "enemy/enemy.gltf";
    Vector4 color_ = {1,1,1,1};
#ifdef _DEBUG
    char modelName_[256];

    bool gui_drawCollider_ = true;
    bool idleFlag_ = true;
#endif // _DEBUG


};