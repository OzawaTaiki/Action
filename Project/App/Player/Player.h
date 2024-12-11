#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/Config/Config.h>

#include <memory>
#include <string>


class Player
{
public:

    Player() = default;
    ~Player() = default;

    void Initialize();
    void Update();
    void Draw(const Camera* _camera);

    void SetFllowCamera(Camera* _camera);

private:
    std::unique_ptr<ObjectModel> model_ = nullptr;

    Vector3 move_ = {};
    float moveSpeed_ = 1.0f;


    std::unique_ptr<Config> config_ = nullptr;
    std::string modelPath_ = "Sphere/Sphere.obj";

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};