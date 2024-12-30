#pragma once

#include <App/Weapon/IWeapon.h>
#include <Rendering/Model/ObjectModel.h>
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

#ifdef _DEBUG
    void SetDrawCollider(bool _draw) { drawCollider_ = _draw; }
#endif // _DEBUG


private:

    std::unique_ptr<ObjectModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;
    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    std::string modelPath_ = "Weapon/Sword.gltf";

#ifdef _DEBUG
    bool drawCollider_ = false;
    char modelName_[256];
#endif


};

