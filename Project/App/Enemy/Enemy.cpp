#include "Enemy.h"

uint32_t Enemy::enemyCount_ = 0;

Enemy::Enemy()
{
    enemyID_ = enemyCount_++;
}

void Enemy::Initialize()
{

    std::string enemyName = "Enemy_" + std::to_string(enemyID_);
    jsonBinder_ = std::make_unique<JsonBinder>(enemyName, "Resources/Data/Parameter/");

    jsonBinder_->RegisterVariable("hp", &hp_);
    jsonBinder_->RegisterVariable("color", &color_);
    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);

    if (modelPath_.empty())
        modelPath_ = "";

    model_ = std::make_unique<ObjectModel>();
    model_->Initialize(modelPath_, enemyName);


}

void Enemy::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG


    model_->Update();
}

void Enemy::Draw(const Camera* _camera)
{
    model_->Draw(_camera, color_);
}

void Enemy::ImGui()
{
}
