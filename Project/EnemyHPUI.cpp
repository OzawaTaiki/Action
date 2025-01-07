#include "EnemyHPUI.h"

void EnemyHPUI::Initialize(float _maxHp, const Vector4& _barColor, const Vector4& _frameColor, const WorldTransform* _parent)
{
    maxHP_ = _maxHp;
    hp_ = maxHP_;
    barColor_ = _barColor;
    frameColor_ = _frameColor;

    bar_ = std::make_unique<ObjectModel>();
    bar_->Initialize("UI/EnemyHPBar.gltf", "EnemyHPBar");
    bar_->SetParent(_parent);

    frame_ = std::make_unique<ObjectModel>();
    frame_->Initialize("UI/EnemyHPFrame.gltf", "EnemyHPFrame");
    frame_->SetParent(_parent);
}

void EnemyHPUI::Draw(const Camera* _camera)
{
    Update();
    bar_->Draw(_camera, barColor_);
    frame_->Draw(_camera, frameColor_);
}

void EnemyHPUI::Update()
{
    ratio_ = hp_ / maxHP_;

}
