#include "TitleScene.h"
#include <Features/Model/Manager/ModelManager.h>
#include <Features/Sprite/Sprite.h>
#include <Debug/ImGuiManager.h>
#include <Features/Scene/Manager/SceneManager.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>


std::unique_ptr<BaseScene>TitleScene::Create()
{
    return std::make_unique<TitleScene>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,5,-16 };
    SceneCamera_.rotate_ = { 0.26f,0,0 };
    SceneCamera_.UpdateMatrix();

    debugCamera_.Initialize();


    plane_ = std::make_unique<ObjectModel>();
    plane_->Initialize("Tile/Tile.gltf", "Ground");
    plane_->GetUVTransform().SetScale(100, 100);

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

    uint32_t textureHandle = TextureManager::GetInstance()->Load("TitleText.png");
    titleSprite_ = Sprite::Create(textureHandle, { .5f, .5f });
    titleSprite_->SetSize({ 320,160 });
    titleSprite_->translate_ = { 640,200 };

    textureHandle = TextureManager::GetInstance()->Load("PressText.png");
    pressEnterSprite_ = Sprite::Create(textureHandle, { .5f, .5f });
    pressEnterSprite_->SetSize({ 480,160 });
    pressEnterSprite_->translate_ = { 640,500 };

    titleText_ = std::make_unique<UISprite>();
    titleText_->Initialize("TitleText");

    pressText_ = std::make_unique<UISprite>();
    pressText_->Initialize("PressText");


    pPlayer_ = std::make_unique<Player>();
    pPlayer_->Initialize();

}

void TitleScene::Update()
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


    titleText_->Update();
    pressText_->Update();
#endif // _DEBUG

    plane_->Update();

    titleSprite_->Update();
    pressEnterSprite_->Update();

    if (Input::GetInstance()->IsKeyTriggered(DIK_SPACE) ||
        Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
        SceneManager::ReserveScene("Game");

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
        ParticleManager::GetInstance()->Update(debugCamera_.rotate_);
    }
    else
    {
        SceneCamera_.rotate_ = { 0.26f,angle_,0 };

        Matrix4x4 matRot = MakeRotateMatrix(SceneCamera_.rotate_);
        Vector3 rotVelo = TransformNormal({ 0,0,-16 }, matRot);

        SceneCamera_.translate_ = rotVelo;
        SceneCamera_.translate_.y = 5;

        angle_ += angleSpeed_;

        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
        ParticleManager::GetInstance()->Update(SceneCamera_.rotate_);
    }

}

void TitleScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();
    plane_->Draw(&SceneCamera_, { 1,1,1,1 });

//-------------------------------------------------------------------------
    ModelManager::GetInstance()->PreDrawForAnimationModel();
    pPlayer_->Draw(&SceneCamera_);

//-------------------------------------------------------------------------
    Sprite::PreDraw();

    titleText_->Draw();
    pressText_->Draw();

//-------------------------------------------------------------------------
    ParticleManager::GetInstance()->Draw(&SceneCamera_);
    lineDrawer_->Draw();
}

#ifdef _DEBUG
#include <imgui.h>
void TitleScene::ImGui()
{
    ImGui::DragFloat3("title_trans", &titleSprite_->translate_.x, 0.1f);
    ImGui::DragFloat3("title_scale", &titleSprite_->scale_.x, 0.1f);

    ImGui::DragFloat3("press_trans", &pressEnterSprite_->translate_.x, 0.1f);
    ImGui::DragFloat3("press_scale", &pressEnterSprite_->scale_.x, 0.1f);


}
#endif // _DEBUG
