#include "GameScene.h"
#include <Rendering/Model/ModelManager.h>
#include <Rendering/Sprite/Sprite.h>
#include <UI/ImGuiManager/ImGuiManager.h>
#include <Framework/eScene/SceneManager.h>

std::unique_ptr<BaseScene>GameScene::Create()
{
    return std::make_unique<GameScene>();
}
GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,5,-20 };
    SceneCamera_.rotate_ = { 0.26f,0,0 };
    SceneCamera_.UpdateMatrix();

    debugCamera_.Initialize();

    plane_ = std::make_unique<ObjectModel>();
    plane_->Initialize("Tile/Tile.gltf", "Ground");
    plane_->uvScale_ = { 100,100 };

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

#pragma region App

    pPlayer_ = std::make_unique<Player>();
    pPlayer_->Initialize();



#pragma endregion
}

void GameScene::Update()
{

#ifdef _DEBUG
    ImGui();

    if(Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
       Input::GetInstance()->IsKeyPressed(DIK_RSHIFT))
        enableDebugCamera_ = !enableDebugCamera_;
#endif // _DEBUG

    plane_->Update();

    pPlayer_->Update();

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
        ParticleManager::GetInstance()->Update(debugCamera_.rotate_);
    }
    else
    {
        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
        ParticleManager::GetInstance()->Update(SceneCamera_.rotate_);
    }

}

void GameScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();
    plane_->Draw(&SceneCamera_, { 1,1,1,1 });

    pPlayer_->Draw(&SceneCamera_);

//-------------------------------------------------------------------------
    ModelManager::GetInstance()->PreDrawForAnimationModel();


//-------------------------------------------------------------------------
    Sprite::PreDraw();


//-------------------------------------------------------------------------
    ParticleManager::GetInstance()->Draw(&SceneCamera_);
    lineDrawer_->Draw();
}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG
