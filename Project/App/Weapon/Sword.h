#pragma once

#include <App/Weapon/IWeapon.h>
#include <Features/Model/ObjectModel.h>
#include <Features/Model/AnimationModel.h>

#include <Features/Collision/Collider/Collider.h>
#include <Features/Json/JsonBinder.h>

#include <App/Effects/SlashEffect.h>

#include <memory>
#include <json.hpp>

class Sword : public BaseWeapon
{
public:
    Sword() = default;
    ~Sword() override = default;

    void Initialize() override;
    void Update() override;
    void Draw(const Camera* _camera) override;
    void ImGui() override;
    void OnCollision(const Collider* _other);

    void SetParent(const WorldTransform* _parent) { model_->SetParent(_parent); }
    void RegsitCollider();

    bool CanCommbo() const { return isWaitingForIdle_; }
    bool CanAttack() const { return isWaitingForIdle_ || isIdle_; }
    bool IsIdle()const { return isIdle_;  }

#ifdef _DEBUG
    void SetDrawCollider(bool _draw) { gui_drawCollider_ = _draw; }
#endif // _DEBUG

private:


    float elapsedTime = 0.0f;               // 経過時間
    bool isActionActive_ = false;           // 行動中か
    bool isIdle_ = true;                    // Idle状態か
    bool isWaitingForIdle_ = true;          // Idle状態に戻るまで待機中か



    std::array<std::unique_ptr<SlashEffect>, 32> slashEffect_;
    std::array<bool, 32> isSlashEffectActive_ = { false };


    std::string currentAction_ = "";

    std::unique_ptr<AnimationModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;
    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    std::string modelPath_ = "Weapon/Sword.gltf";


#ifdef _DEBUG
    bool gui_drawCollider_ = false;
    bool isDrawCollider_ = false;
    char modelName_[256];
#endif


};

