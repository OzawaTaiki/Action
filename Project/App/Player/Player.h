#pragma once

#include <Features/Model/ObjectModel.h>
#include <Features/Camera/Camera/Camera.h>
#include <Features/Json/JsonBinder.h>
#include <Features/Collision/Collider/Collider.h>

#include <App/Weapon/Sword.h>
#include <App/CircleShadow/CircleShadow.h>



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

    void OnCollision(const Collider* _other);

    bool IsAlive()const { return isAlive_; }

    const WorldTransform* GetWorldTransform() { return model_->GetWorldTransform(); }
    Vector3 GetPosition() { return model_->GetWorldTransform()->GetWorldPosition(); }

private:
    void UpdateRightArmTrans();

    void Move(const Matrix4x4& _cameraRotMat);
    void Rotation();
    void Attack();

    void Idle();

    WorldTransform rightArm_ = {};
    std::function<void()> f_currentState_ = nullptr;

    std::unique_ptr<AnimationModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;


    Vector3 move_ = {};
    float moveSpeed_ = 1.0f;

    Vector3 targetDirection_ = {};
    Quaternion targetQuaternion_ = Quaternion::Identity();

#pragma region Weapon

    std::unique_ptr<Sword> weapon_ = nullptr;

#pragma endregion / Weapon End

#pragma region Attack

    std::optional<int32_t> currentAttack_ = std::nullopt;
    std::optional<int32_t> nextAttack_ = std::nullopt;

    // trigger
    // attaking
    // canCommbo
    //

    bool isTrigger_ = false;    // 攻撃ボタンが押されたかどうか
    bool isAttacking_ = false;  // 攻撃中かどうか
    bool canCommbo_ = false;    // コンボ可能かどうか
    bool canAttack_ = true;     // 攻撃可能かどうか
    bool toIdle_ = false;        // 攻撃後にIdle状態に戻るかどうか

    std::vector<std::string> attackNames_ = { "Attack_01","Attack_02" ,"Attack_03" };


#pragma endregion / Attack End

    std::unique_ptr<CircleShadow> circleShadow_ = nullptr;

    float maxHP_ = 100.0f;
    float hp_ = 0.0f;
    bool isAlive_ = true;


    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;
    std::string modelPath_ = "Sphere/Sphere.obj";

#ifdef _DEBUG
    void ImGui();

    bool gui_drawCollider_ = true;
#endif // _DEBUG

};
