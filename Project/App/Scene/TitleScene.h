#pragma once

#include <Features/Scene/Interface/BaseScene.h>

#include <Features/Camera/Camera/Camera.h>
#include <Features/Camera/DebugCamera/DebugCamera.h>
#include <Features/Model/ObjectModel.h>
#include <Features/Sprite/Sprite.h>
#include <Features/Effect/Manager/ParticleManager.h>
#include <Features/LineDrawer/LineDrawer.h>
#include <System/Input/Input.h>

#include <memory>

class TitleScene :public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    TitleScene() = default;
    ~TitleScene() override;

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

    Sprite* titleSprite_ = nullptr;
    Sprite* pressEnterSprite_ = nullptr;


#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};
