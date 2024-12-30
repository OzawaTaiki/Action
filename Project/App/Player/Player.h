#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/JsonBinder/JsonBinder.h>
#include <Physics/Collision/Collider.h>


#include <memory>
#include <string>


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

    std::unique_ptr<ObjectModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;


    Vector3 move_ = {};
    float moveSpeed_ = 1.0f;

    float targetDirection_ = {};


    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;
    std::string modelPath_ = "Sphere/Sphere.obj";

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};