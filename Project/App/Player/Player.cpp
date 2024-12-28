#include "Player.h"
#include <Systems/Input/Input.h>


void Player::Initialize()
{
    jsonBinder_ = std::make_unique<JsonBinder>("Player", "Resources/Data/Parameter/");

    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("MoveSpeed", &moveSpeed_);

    if (modelPath_.empty())
        modelPath_ = "Sphere/Sphere.obj";

    model_ = std::make_unique<ObjectModel>();
    model_->Initialize(modelPath_, "Player");

}

void Player::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG

    Input::GetInstance()->GetMove(move_, moveSpeed_);

    // 移動がない場合コントローラー
    if (move_.x == 0 && move_.y == 0 && move_.z == 0)
    {
        Vector2 padMove = Input::GetInstance()->GetPadLeftStick();
        move_.x = padMove.x;
        move_.z = padMove.y;

        move_ *= moveSpeed_;
    }

    move_.y = 0;
    model_->translate_ += move_ ;

    model_->Update();
}

void Player::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });
}

#ifdef _DEBUG
#include <imgui.h>
void Player::ImGui()
{
    ImGui::Begin("Player");
    ImGui::Text("MoveSpeed");
    ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
    if (ImGui::Button("Save"))
        jsonBinder_->Save();

    ImGui::End();

}
#endif // _DEBUG
