#pragma once

#include <Framework/eScene/BaseScene.h>

#include <Framework/Camera/Camera.h>
#include <Framework/Camera/DebugCamera.h>
#include <Rendering/Model/ObjectModel.h>
#include <Framework/Particle/ParticleManager.h>
#include <Rendering/LineDrawer/LineDrawer.h>
#include <Systems/Input/Input.h>
#include <Rendering/Sprite/Sprite.h>

#include <memory>

class ClearScene :public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    ClearScene() = default;
    ~ClearScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:

    // シーン関連
    Camera SceneCamera_ = {};
    DebugCamera debugCamera_ = {};
    bool enableDebugCamera_ = false;

    LineDrawer* lineDrawer_ = nullptr;
    Input* input_ = nullptr;
    ParticleManager* particleManager_ = nullptr;

    Sprite* clearSprite_ = nullptr;


#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};