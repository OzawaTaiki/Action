#include "Player.h"
#include <Systems/Input/Input.h>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/VectorFunction.h>
#include <Physics/Math/MyLib.h>


void Player::Initialize()
{
    jsonBinder_ = std::make_unique<JsonBinder>("Player", "Resources/Data/Parameter/");

    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("MoveSpeed", &moveSpeed_);

    if (modelPath_.empty())
        modelPath_ = "Sphere/Sphere.obj";

    model_ = std::make_unique<ObjectModel>();
    model_->Initialize(modelPath_, "Player");

    collider_ = std::make_unique<Collider>();
    collider_->SetBoundingBox(Collider::BoundingBox::AABB_3D);
    collider_->SetShape(model_->GetMin(), model_->GetMax());
    collider_->SetAtrribute("Player");
    collider_->SetMask("Player");
    collider_->SetGetWorldMatrixFunc([this]() {return model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });
    collider_->SetReferencePoint({ 0,0,0 });

}

void Player::Update(const Vector3& _cameraroate)
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG


    Matrix4x4 cameraRotMat = MakeRotateMatrix(_cameraroate);
    Move(cameraRotMat);

    Rotation();

    collider_->RegsterCollider();

    model_->Update();
}

void Player::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });
#ifdef _DEBUG
    collider_->Draw();
#endif // _DEBUG

}

void Player::Move(const Matrix4x4& _cameraRotMat)
{
    Input::GetInstance()->GetMove(move_, moveSpeed_);

    // 移動がない場合コントローラー
    if (move_ == Vector3(0, 0, 0))
    {
        Vector2 padMove = Input::GetInstance()->GetPadLeftStick();
        move_.x = padMove.x;
        move_.z = padMove.y;

        move_ *= moveSpeed_;
    }

    Vector3 normalizeMove = move_.Normalize();

    move_ = TransformNormal(move_, _cameraRotMat);
    move_.y = 0;

    model_->translate_ += move_;

    if (normalizeMove != Vector3(0, 0, 0))
    {
        normalizeMove = TransformNormal(normalizeMove, _cameraRotMat);
        targetDirection_ = std::atan2(normalizeMove.x, normalizeMove.z);
    }
}

// TODO : Playerの攻撃（とりあえず単発） 敵にダメージを入れられるように。敵HP0以下で削除
// TODO : 敵の攻撃（とりあえず単発） Playerにダメージを入れられるように。PlayerHP0以下でゲームオーバー

void Player::Rotation()
{
    model_->rotate_.y = LerpShortAngle(model_->rotate_.y, targetDirection_, 0.1f);
}

#ifdef _DEBUG
#include <imgui.h>
void Player::ImGui()
{
    ImGui::Begin("Player");
    ImGui::Text("MoveSpeed");
    ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);

    static char modelName[256];
    ImGui::InputText("ModelPath", modelName, 256);
    if (ImGui::Button("Set"))
    {
        modelPath_ = modelName;
        model_->SetModel(modelPath_);
    }
    ImGui::Text("ModelPath : %s", modelPath_.c_str());


    if (ImGui::Button("Save"))
        jsonBinder_->Save();



    ImGui::End();

}
#endif // _DEBUG
