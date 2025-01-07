#pragma once

#include <Physics/Math/Vector2.h>
#include <Physics/Math/Vector4.h>

#include <Rendering/Model/ObjectModel.h>

class WolrdTransform;
class Camera;
class EnemyHPUI
{
public:
    EnemyHPUI() = default;
    ~EnemyHPUI() = default;
    void Initialize(float _maxHp, const Vector4& _barColor, const Vector4& _frameColor,const WorldTransform* _parent);
    void Draw(const Camera* _camera);


private:

    void Update();
    float hp_ = 0.0f;
    float maxHP_ = 0.0f;
    float ratio_ = 0.0f;

    std::unique_ptr<ObjectModel> bar_ = nullptr;
    std::unique_ptr<ObjectModel> frame_ = nullptr;

    Vector4 barColor_;
    Vector4 frameColor_;

    const WolrdTransform* worldTransform_ = nullptr;

};

