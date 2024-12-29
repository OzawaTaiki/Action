#include "FllowCamera.h"
#include "App/Player/Player.h"
#include <Systems/Input/Input.h>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/VectorFunction.h>

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

    Rotate();

    //camera_.translate_ = target_->GetWorldTransform()->GetWorldPosition() + offsetPos_;
    //camera_.rotate_ = offsetRot_;


    camera_.Update(false);
    camera_.UpdateMatrix();

}

void FollowCamera::Rotate()
{
    // スティックの入力を取得
    Vector2 padRightStick = Input::GetInstance()->GetPadRightStick();

    // 取得した値をPlayerを基準に回転させる
    camera_.rotate_.y += padRightStick.x * rotationSpeed_; // 左右
    camera_.rotate_.x -= padRightStick.y * rotationSpeed_; // 上下

    Matrix4x4 rotMat = MakeRotateMatrix(camera_.rotate_);
    Vector3 offset = TransformNormal(offsetPos_, rotMat);

    camera_.translate_ = target_->GetWorldTransform()->GetWorldPosition() + offset;


}
