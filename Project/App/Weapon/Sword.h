#pragma once

#include <App/Weapon/IWeapon.h>
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>

#include <Physics/Collision/Collider.h>
#include <Systems/JsonBinder/JsonBinder.h>

#include <memory>

class Sword : public BaseWeapon
{
public:
    Sword() = default;
    ~Sword() override = default;

    void Initialize() override;
    void Update() override;
    void Draw(const Camera* _camera) override;
    void ImGui() override;
    void OnCollision(const Collider* _other) {}

    void SetParent(const WorldTransform* _parent) { model_->SetParent(_parent); }

    bool ToTargetQuaternion(const Quaternion& _targetQuaternion, float _duration);


#ifdef _DEBUG
    void SetDrawCollider(bool _draw) { drawCollider_ = _draw; }
#endif // _DEBUG

private:


    float elapsedTime = 0.0f;               // 経過時間
    bool isActionActive_ = false;           // 行動中か

    // targetQuaternion_に向かって補間する

    // 右上から左下への袈裟斬り
    //void BeginKesagiri();
    //void Kesagiri();




    std::unique_ptr<AnimationModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;
    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    std::string modelPath_ = "Weapon/Sword.gltf";

#ifdef _DEBUG
    bool drawCollider_ = false;
    char modelName_[256];
#endif


};

