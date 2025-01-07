#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Physics/Collision/Collider.h>

#include <memory>

class Camera;
class BaseEnemy
{
public:

    BaseEnemy() = default;
    virtual ~BaseEnemy() {};
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw(const Camera* _camera) = 0;


    bool IsAlive()const { return isAlive_; }

    virtual void ImGui() = 0;


protected:

    float hp_;
    bool isAlive_;
    std::unique_ptr<ObjectModel> model_ = nullptr;
    std::unique_ptr<Collider> collider_ = nullptr;


};