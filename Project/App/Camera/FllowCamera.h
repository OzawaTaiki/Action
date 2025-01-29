#pragma once

#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Features/Camera/Camera/Camera.h>
#include <Features/Model/Transform/WorldTransform.h>

class Player;

class FollowCamera
{
public:

    FollowCamera() = default;
    ~FollowCamera() = default;

    void Initialize(const Vector3& _offsetPos);
    void Update();

    void SetOffsetPos(const Vector3& _offset) { offsetPos_ = _offset; }
    void SetRot(const Vector3& _offset) { offsetRot_ = _offset; }

    Matrix4x4 GetProjection()const { return camera_.matProjection_; }
    Matrix4x4 GetView()const { return camera_.matView_; }

    Vector3 GetRotation()const { return camera_.rotate_; }

    void SetPlayer(Player* _player) { target_ = _player; }


private:

    void Rotate();

    Player* target_ = nullptr;
    float rotationSpeed_ = 0.05f;

    Vector3 offsetPos_ = {};
    Vector3 offsetRot_ = {};

    Camera camera_ = {};

};
