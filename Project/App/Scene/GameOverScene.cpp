#include "GameOverScene.h"
#include <Features/Model/Manager/ModelManager.h>
#include <Features/Sprite/Sprite.h>
#include <Debug/ImGuiManager.h>
#include <Features/Scene/Manager/SceneManager.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>


std::unique_ptr<BaseScene>GameOverScene::Create()
{
    return std::make_unique<GameOverScene>();
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,5,-20 };
    SceneCamera_.rotate_ = { 0.26f,0,0 };
    SceneCamera_.UpdateMatrix();

    debugCamera_.Initialize();

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

    uint32_t textureHandle = TextureManager::GetInstance()->Load("GameOverText.png");
    gameOverSprite_ = Sprite::Create(textureHandle, { .5f, .5f });
    gameOverSprite_->SetSize({ 320,160 });
    gameOverSprite_->translate_ = { 640,200 };
}

void GameOverScene::Update()
{

#ifdef _DEBUG
    ImGui();

    if(Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
       Input::GetInstance()->IsKeyPressed(DIK_RSHIFT))
        enableDebugCamera_ = !enableDebugCamera_;
#endif // _DEBUG

    if (Input::GetInstance()->IsKeyTriggered(DIK_SPACE) ||
        Input::GetInstance()->IsPadReleased(PadButton::iPad_A))
        SceneManager::ReserveScene("Title");

    gameOverSprite_->Update();

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

void GameOverScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();

//-------------------------------------------------------------------------
    ModelManager::GetInstance()->PreDrawForAnimationModel();


//-------------------------------------------------------------------------
    Sprite::PreDraw();
    gameOverSprite_->Draw();

//-------------------------------------------------------------------------
    ParticleManager::GetInstance()->Draw(&SceneCamera_);
    lineDrawer_->Draw();
}

#ifdef _DEBUG
#include <imgui.h>
void GameOverScene::ImGui()
{

}
#endif // _DEBUG
