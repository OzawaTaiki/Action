#pragma once

#include <Features/Scene/Interface/BaseScene.h>

#include <Features/Camera/Camera/Camera.h>
#include <Features/Camera/DebugCamera/DebugCamera.h>
#include <Features/Model/ObjectModel.h>
#include <Features/Json/JsonBinder.h>
#include <Features/Effect/Manager/ParticleManager.h>
#include <Features/LineDrawer/LineDrawer.h>
#include <System/Input/Input.h>

#include <memory>

#include <App/Player/Player.h>
#include <App/Enemy/EnemyManager.h>
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
    EnemyManager* pEnemyManager_ = nullptr;



#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG

};
