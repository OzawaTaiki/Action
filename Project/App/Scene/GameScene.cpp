#include "GameScene.h"
#include <Features/Model/Manager/ModelManager.h>
#include <Features/Sprite/Sprite.h>
#include <Debug/ImGuiManager.h>
#include <Features/Scene/Manager/SceneManager.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>
#include <Features/Collision/Manager/CollisionManager.h>

std::unique_ptr<BaseScene>GameScene::Create()
{
    return std::make_unique<GameScene>();
}
GameScene::~GameScene()
{
    ParticleManager::GetInstance()->ClearGroup();
}

void GameScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,2.5f,-20 };
    SceneCamera_.rotate_ = { 0.26f,0,0 };
    SceneCamera_.UpdateMatrix();

    debugCamera_.Initialize();

    plane_ = std::make_unique<ObjectModel>();
    plane_->Initialize("Tile/Tile.gltf", "Ground");
    plane_->GetUVTransform().SetScale(100, 100);

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

#pragma region App

    SceneCamera_.SetTimeChannel("Effects");

    pPlayer_ = std::make_unique<Player>();
    pPlayer_->Initialize();

    followCamera_ = std::make_unique<FollowCamera>();
    followCamera_->Initialize({ 0,2.0f,-20 });
    followCamera_->SetPlayer(pPlayer_.get());

    pEnemyManager_ = EnemyManager::GetInstance();
    pEnemyManager_->Initialize();
    pEnemyManager_->SetPlayerPosition(pPlayer_->GetWorldTransform());

    skyDome_ = std::make_unique<ObjectModel>();
    skyDome_->Initialize("skydome/skydome.obj", "SkyDome");
    skyDome_->scale_ = { 50,50 ,50 };
    skyDome_->GetMaterial()->enableLighting_ = false;

    skyDomeTexture_ = TextureManager::GetInstance()->Load("skydome.png");

    pPlayer_->SetCamera(followCamera_->GetCamera());


#pragma endregion
}

void GameScene::Update()
{
    CollisionManager::GetInstance()->ResetColliderList();

#ifdef _DEBUG
    ImGui();

    if(Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
       Input::GetInstance()->IsKeyPressed(DIK_RSHIFT))
        enableDebugCamera_ = !enableDebugCamera_;
#endif // _DEBUG

    if(EnemyManager::GetInstance()->Clear())
        SceneManager::ReserveScene("Clear");

    if(!pPlayer_->IsAlive())
        SceneManager::ReserveScene("gameOver");

    skyDome_->Update();
    plane_->Update();

    followCamera_->Update();
    pPlayer_->Update();
    pEnemyManager_->Update();

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        ParticleManager::GetInstance()->Update(debugCamera_.rotate_);
    }
    else
    {
        SceneCamera_.matView_ = followCamera_->GetView();
        ParticleManager::GetInstance()->Update(followCamera_->GetRotation());
    }

    CollisionManager::GetInstance()->CheckAllCollision();
    SceneCamera_.TransferData();
}

void GameScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();
    skyDome_->Draw(&SceneCamera_, skyDomeTexture_, { 1,1,1,1 });
    plane_->Draw(&SceneCamera_, { 1,1,1,1 });

    pEnemyManager_->Draw(&SceneCamera_);
    pPlayer_->Draw(&SceneCamera_);

//-------------------------------------------------------------------------
    ModelManager::GetInstance()->PreDrawForAnimationModel();


//-------------------------------------------------------------------------
    Sprite::PreDraw();
    EnemyManager::GetInstance()->DrawFront(&SceneCamera_);

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
