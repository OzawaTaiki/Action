#pragma once

#include <Features/Scene/Interface/BaseScene.h>

#include <Features/Camera/Camera/Camera.h>
#include <Features/Camera/DebugCamera/DebugCamera.h>
#include <Features/Model/ObjectModel.h>
#include <Features/Effect/Manager/ParticleManager.h>
#include <Features/LineDrawer/LineDrawer.h>
#include <System/Input/Input.h>
#include <Features/Sprite/Sprite.h>
#include <Features/UI/UISprite.h>

#include <App/Player/Player.h>


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

    std::unique_ptr<UISprite> clearText_ = nullptr;

    Vector2 startPos = { 640,-110 };
    Vector2 velo = { 0,0 };
    float e = .5f;
    uint32_t boundCount = 0;

    std::unique_ptr<ObjectModel> plane_ = nullptr;
    std::unique_ptr<Player> pPlayer_ = nullptr;
    bool playerUpdate_ = false;

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};
