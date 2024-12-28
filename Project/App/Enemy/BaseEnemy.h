#pragma once

#include <Rendering/Model/ObjectModel.h>

class Camera;
class BaseEnemy
{
public:

    BaseEnemy() = default;
    virtual ~BaseEnemy() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw(const Camera* _camera) = 0;

    virtual void ImGui() = 0;


protected:

    float hp_;
    bool isAlive_;
    std::unique_ptr<ObjectModel> model_ = nullptr;




};