#include "SampleFramework.h"

#include "SampleScene.h"
#include <Features/Scene/ParticleTestScene.h>
#include <App/Essential/SceneFactory/SceneFactory.h>
#include <App/Scene/SceneTransition.h>

void SampleFramework::Initialize()
{
    Framework::Initialize();


    JsonHub::GetInstance()->Initialize("Resources/Data/");


    sceneManager_->SetSceneFactory(new SceneFactory());

    // 最初のシーンで初期化
    sceneManager_->Initialize("Title");

    sceneManager_->SetTransition(std::make_unique<SceneTransition>());
}

void SampleFramework::Update()
{
    Framework::Update();

    collisionManager_->ResetColliderList();

    //========== 更新処理 =========

    sceneManager_->Update();
    //particleManager_->Update(); TODO ; 引数のカメラの回転をなんとかしたい
    collisionManager_->CheckAllCollision();

    //=============================
}

void SampleFramework::Draw()
{
    Framework::PreDraw();

    // ========== 描画処理 ==========

    sceneManager_->Draw();

    lineDrawer_->Draw();
    //=============================

    Framework::PostDraw();

    // 後にupdateに
    sceneManager_->ChangeScene();

}

void SampleFramework::Finalize()
{
    Framework::Finalize();
}
