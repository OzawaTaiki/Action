#pragma once

#include <Features/Model/ObjectModel.h>

class CircleShadow
{
public:
    CircleShadow() = default;
    ~CircleShadow() = default;

    void Initialize(const WorldTransform* _wt);
    void Draw(const Camera* _camera);

    void SetScale(const Vector3& _scale) { circle_->scale_ = _scale; }

private:

    std::unique_ptr<ObjectModel> circle_;


};
