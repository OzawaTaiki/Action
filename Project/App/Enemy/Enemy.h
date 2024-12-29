#pragma once

#include "BaseEnemy.h"
#include <Systems/JsonBinder/JsonBinder.h>

class Enemy : public BaseEnemy
{
public:
    Enemy();
    ~Enemy() = default;

    void Initialize() override;
    void Update() override;
    void Draw(const Camera* _camera) override;

    void ImGui() override;

    void OnCollision(const Collider* _other);

private:
    static uint32_t enemyCount_;

    uint32_t enemyID_;

    std::unique_ptr<JsonBinder> jsonBinder_;
    std::string modelPath_ = "Sphere/Sphere.obj";
    Vector4 color_ = {1,1,1,1};


};