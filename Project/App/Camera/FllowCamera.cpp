#include "FllowCamera.h"
#include <stdexcept>

void FollowCamera::Initialize(const Vector3& _offsetPos)
{
    camera_.Initialize();

    offsetPos_ = _offsetPos;
    offsetRot_ = { 0.26f,0.0f,0.0f };
}

void FollowCamera::Update()
{
#ifdef _DEBUG
    ImGui::SeparatorText("FollowCamera");

    ImGui::DragFloat3("offsetPos", &offsetPos_.x, 0.01f);
    ImGui::DragFloat3("Rot", &offsetRot_.x, 0.01f);

#endif // _DEBUG


    if (target_ == nullptr)
        throw std::runtime_error("target is nullptr");

    camera_.translate_ = target_->GetWorldPosition() + offsetPos_;
    camera_.rotate_ = offsetRot_;


    camera_.Update(false);
    camera_.UpdateMatrix();

}
