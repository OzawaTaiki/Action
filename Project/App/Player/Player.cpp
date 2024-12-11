#include "Player.h"
#include <Systems/Input/Input.h>


void Player::Initialize()
{
    config_ = std::make_unique<Config>("Player", "Resources/Data/Parameter/");

    config_->SetVariable("ModelPath", &modelPath_);
    config_->SetVariable("MoveSpeed", &moveSpeed_);

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

    move_.y = 0;
    model_->translate_ += move_ * moveSpeed_;

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
        config_->Save();

    ImGui::End();

}
#endif // _DEBUG
