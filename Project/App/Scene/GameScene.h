#pragma once

#include <Framework/eScene/BaseScene.h>

#include <Framework/Camera/Camera.h>
#include <Framework/Camera/DebugCamera.h>
#include <Rendering/Model/ObjectModel.h>
#include <Systems/Config/ConfigManager.h>
#include <Framework/Particle/ParticleManager.h>
#include <Rendering/LineDrawer/LineDrawer.h>
#include <Systems/Input/Input.h>

#include <memory>

#include <App/Player/Player.h>
#include <App/Camera/FllowCamera.h>


class GameScene :public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    GameScene() = default;
    ~GameScene() override;

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

    std::unique_ptr<ObjectModel> plane_ = nullptr;

    std::unique_ptr<Player> pPlayer_;
    std::unique_ptr<FollowCamera> followCamera_ = nullptr;



#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};