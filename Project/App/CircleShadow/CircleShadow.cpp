#include "CircleShadow.h"

void CircleShadow::Initialize(const WorldTransform* _wt)
{
    circle_ = std::make_unique<ObjectModel>();
    circle_->Initialize("Sphere/Sphere.obj", "CircleShadow");
    circle_->translate_.y = 0.01f;
    circle_->scale_.y = 0.0001f;

    circle_->SetParent(_wt);
}

void CircleShadow::Draw(const Camera* _camera)
{
    circle_->Update();
    circle_->Draw(_camera, { 0,0,0,1 });
}

