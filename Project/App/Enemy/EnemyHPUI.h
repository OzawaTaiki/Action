#pragma once

#include <Physics/Math/Vector2.h>
#include <Physics/Math/Vector4.h>

#include <Rendering/Sprite/Sprite.h>
#include <Systems/JsonBinder/JsonBinder.h>

class WorldTransform;
class Camera;
class EnemyHPUI
{
public:
    EnemyHPUI() = default;
    ~EnemyHPUI() = default;
    void Initialize(float _maxHp, const Vector4& _barColor, const Vector4& _frameColor,const WorldTransform* _parent,const float* _hp);
    void Draw(const Camera* _camera);


private:

    void Update(const Camera* _camera);
    float maxHP_ = 0.0f;
    float ratio_ = 0.0f;
    const float* hp_ = nullptr;

    Sprite* bar_ = nullptr;
    Sprite* frame_ = nullptr;

    Vector4 barColor_;
    Vector4 frameColor_;

    Vector2 offset = {};

    Vector2 barSize_ = {};
    Vector2 frameSize_ = {};

    const WorldTransform* worldTransform_ = nullptr;

    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    Vector2 GetScreenPos(const Camera* _camera, const Vector3& _pos);
};


