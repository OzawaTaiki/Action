#include "EnemyHPUI.h"

#include <Math/Matrix/MatrixFunction.h>
#include <Math/Vector/VectorFunction.h>
#include <Core/WinApp/WinApp.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>
#include <Features/Camera/Camera/Camera.h>
#include <Features/Model/Transform/WorldTransform.h>

void EnemyHPUI::Initialize(float _maxHp, const Vector4& _barColor, const Vector4& _frameColor, const WorldTransform* _parent,const float* _hp)
{
    maxHP_ = _maxHp;
    barColor_ = _barColor;
    frameColor_ = _frameColor;

    hp_ = _hp;

    uint32_t textureHandle = TextureManager::GetInstance()->Load("white.png");
    bar_ = Sprite::Create(textureHandle);
    barSize_={ 64,16 };

    textureHandle = TextureManager::GetInstance()->Load("EnemyHPUI.png");
    frame_ = Sprite::Create(textureHandle);
    frameSize_ = { 64,16 };

    JsonHub::GetInstance()->SetDirectoryPathFromRoot("Data/UI/");

    jsonBinder_ = std::make_unique<JsonBinder>("EnemyHP", "Data/UI/");
    jsonBinder_->RegisterVariable("bar_trans", &bar_->translate_);
    jsonBinder_->RegisterVariable("frame_trans", &frame_->translate_);

    worldTransform_ = _parent;

    offset = { 0,-128 };
}

void EnemyHPUI::Draw(const Camera* _camera)
{
    Update(_camera);
    bar_->Draw(barColor_);
    frame_->Draw(frameColor_);
}

void EnemyHPUI::Update(const Camera* _camera)
{
    ratio_ = *hp_ / maxHP_;

    Vector2 screenPos = GetScreenPos(_camera, worldTransform_->GetWorldPosition());
    bar_->translate_ = { screenPos + offset };
    frame_->translate_ = { screenPos + offset };


#ifdef _DEBUG
    ImGui::Begin("EnemyHPUI");

    ImGui::DragFloat2("offset", &offset.x, 1);


    ImGui::SeparatorText("frame");

    ImGui::DragFloat2("frame_trans", &frame_->translate_.x, 1);
    ImGui::DragFloat2("frame_size", &frameSize_.x, 1);

    ImGui::SeparatorText("bar");

    ImGui::DragFloat2("bar_trans", &bar_->translate_.x, 1);
    ImGui::DragFloat2("bar_size", &barSize_.x, 1);




    ImGui::End();
#endif // _DEBUG

    bar_->SetSize({ barSize_.x * ratio_, barSize_.y });
    frame_->SetSize(frameSize_);



}

Vector2 EnemyHPUI::GetScreenPos(const Camera* _camera, const Vector3& _pos)
{
    Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth_, WinApp::kWindowHeight_, 0, 1);
    Matrix4x4 matViewProjectionViewport = _camera->matView_ * _camera->matProjection_ * matViewport;

    Vector3 position = Transform(_pos, matViewProjectionViewport);
    Vector2 result = { position.x, position.y };

    return result;
}

