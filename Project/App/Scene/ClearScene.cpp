#include "ClearScene.h"
#include <Features/Model/Manager/ModelManager.h>
#include <Features/Sprite/Sprite.h>
#include <Debug/ImGuiManager.h>
#include <Features/Scene/Manager/SceneManager.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>


std::unique_ptr<BaseScene>ClearScene::Create()
{
    return std::make_unique<ClearScene>();
}
ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,5,-13 };
    SceneCamera_.rotate_ = { 0.26f,0,0 };
    SceneCamera_.UpdateMatrix();

    debugCamera_.Initialize();

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

    clearText_ = std::make_unique<UISprite>();
    clearText_->Initialize("ClearText");


    pPlayer_ = std::make_unique<Player>();
    pPlayer_->Initialize();

    plane_ = std::make_unique<ObjectModel>();
    plane_->Initialize("Tile/Tile.gltf", "Ground");

    GameTime::GetChannel("default").SetGameSpeed(1.0f);

}

void ClearScene::Update()
{
    if (!playerUpdate_)
    {
        pPlayer_->Update();
        pPlayer_->Update();
        playerUpdate_ = true;
    }

#ifdef _DEBUG
    ImGui();

    if(Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
       Input::GetInstance()->IsKeyPressed(DIK_RSHIFT))
        enableDebugCamera_ = !enableDebugCamera_;
    clearText_->Update();
#endif // _DEBUG

    if (Input::GetInstance()->IsKeyTriggered(DIK_SPACE) ||
        Input::GetInstance()->IsPadReleased(PadButton::iPad_A))
        SceneManager::ReserveScene("Title");

    plane_->Update();

    velo.y += 3.0f;
    startPos.y += velo.y;
    if (startPos.y > 300)
    {
        boundCount++;
        velo.y = -velo.y * e;
        startPos.y = 300;
    }
    if (boundCount > 3)
    {
        startPos.y = 300;
    }

    clearText_->SetPos(startPos);

    clearSprite_->Update();

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
        //ParticleManager::GetInstance()->Update(debugCamera_.rotate_);
    }
    else
    {
        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
        //ParticleManager::GetInstance()->Update(SceneCamera_.rotate_);
    }

}

void ClearScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();
    plane_->Draw(&SceneCamera_, { 1,1,1,1 });

//-------------------------------------------------------------------------
    ModelManager::GetInstance()->PreDrawForAnimationModel();
    pPlayer_->Draw(&SceneCamera_);

//-------------------------------------------------------------------------
    Sprite::PreDraw();
    clearText_->Draw();

//-------------------------------------------------------------------------
    ParticleManager::GetInstance()->Draw(&SceneCamera_);
    lineDrawer_->Draw();
}

#ifdef _DEBUG
#include <imgui.h>
void ClearScene::ImGui()
{

}
#endif // _DEBUG
