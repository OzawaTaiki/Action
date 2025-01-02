#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/JsonBinder/JsonBinder.h>
#include <Physics/Collision/Collider.h>

#include <App/Weapon/Sword.h>


#include <memory>
#include <string>
#include <functional>


class Player
{
public:

    Player() = default;
    ~Player() = default;

    void Initialize();
    void Update(const Vector3& _cameraroate);
    void Draw(const Camera* _camera);

    void OnCollision(const Collider* _other) {}

    const WorldTransform* GetWorldTransform() { return model_->GetWorldTransform(); }
    Vector3 GetPosition() { return model_->GetWorldTransform()->GetWorldPosition(); }

private:
    void Move(const Matrix4x4& _cameraRotMat);
    void Rotation();

    void Idle();

    std::function<void()> f_currentState_ = nullptr;

    std::unique_ptr<ObjectModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;


    Vector3 move_ = {};
    float moveSpeed_ = 1.0f;

    float targetDirection_ = {};

#pragma region Weapon

    std::unique_ptr<Sword> weapon_ = nullptr;

    Quaternion kesagiriStart_ = Quaternion::Identity();
    Quaternion kesagiriEnd_ = Quaternion::Identity();
    Quaternion beginQuaternion_ = Quaternion::Identity(); // 開始時のクォータニオン

    float kesagiriDuration_ = 0.5f;         // 振り下ろし終わるまでの時間
    float IdleToKesaDuration_ = 0.5f;       // 振り下ろすまでの時間

    bool duringSetup_ = false;              // セットアップ中か

    void BeginKesagiri();
    void Kesagiri();

#pragma endregion / Weapon End

    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;
    std::string modelPath_ = "Sphere/Sphere.obj";

#ifdef _DEBUG
    void ImGui();

    bool drawCollider_ = true;
#endif // _DEBUG

};