#pragma once

class Camera;
class WorldTransform;

class BaseWeapon
{
public:
    BaseWeapon() = default;
    virtual ~BaseWeapon() = default;

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw(const Camera* _camera) = 0;

    virtual void ImGui() = 0;

};
