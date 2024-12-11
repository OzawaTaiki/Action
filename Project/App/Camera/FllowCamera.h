#pragma once

#include <Physics/Math/Vector3.h>
#include <Physics/Math/Matrix4x4.h>
#include <Framework/Camera/Camera.h>
#include <Rendering/Model/WorldTransform.h>

class FollowCamera
{
public:

    FollowCamera() = default;
    ~FollowCamera() = default;

    void Initialize(const Vector3& _offsetPos);
    void Update();

    void SetTarget(const WorldTransform* _target) { target_ = _target; }
    void SetOffsetPos(const Vector3& _offset) { offsetPos_ = _offset; }
    void SetRot(const Vector3& _offset) { offsetRot_ = _offset; }

    Matrix4x4 GetProjection()const { return camera_.matProjection_; }
    Matrix4x4 GetView()const { return camera_.matView_; }

    Vector3 GetRotation()const { return camera_.rotate_; }



private:


    Vector3 offsetPos_ = {};
    Vector3 offsetRot_ = {};

    const WorldTransform* target_ = nullptr;
    Camera camera_ = {};

};