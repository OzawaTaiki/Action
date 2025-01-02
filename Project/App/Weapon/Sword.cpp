#include "Sword.h"
#include <numbers>
#include <Physics/Math/MyLib.h>
#include <Systems/Time/Time.h>


void Sword::Initialize()
{
    model_ = std::make_unique<AnimationModel>();


    jsonBinder_ = std::make_unique<JsonBinder>("Sword", "Resources/Data/Parameter/");
    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("Offset", &model_->translate_);

    //jsonBinder_->RegisterVariable("kesaS", &kesagiriStart_);
    //jsonBinder_->RegisterVariable("kesaE", &kesagiriEnd_);
    //jsonBinder_->RegisterVariable("kesaDuration", &kesagiriDuration_);



    if (modelPath_.empty())
        modelPath_ = "Weapon/Sword.gltf";

    model_->Initialize(modelPath_);

    collider_ = std::make_unique<Collider>();
    collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
    collider_->SetShape(model_->GetMin(), model_->GetMax());
    collider_->SetAtrribute("Sword");
    collider_->SetMask({ "Sword","Player" });
    collider_->SetGetWorldMatrixFunc([this]() {return model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });

    //model_->quaternion_=Quaternion::MakeRotateAxisAngleQuaternion({ 0,1,0 }, std::numbers::pi_v<float> / 4.0f);

}

void Sword::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG


    model_->Update();
}

void Sword::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });

#ifdef _DEBUG
    if (drawCollider_)
        collider_->Draw();
#endif // _DEBUG
}

void Sword::ImGui()
{
#ifdef _DEBUG
    ImGui::Begin("Player");
    ImGui::BeginTabBar("Player");
    if (ImGui::BeginTabItem("Sword"))
    {
        ImGui::Checkbox("DrawCollider", &drawCollider_);
        ImGui::InputText("ModelPath", modelName_, 256);
        ImGui::DragFloat3("Offset", &model_->translate_.x, 0.01f);
        static Vector3 axis = { 0,1,0 };
        static float angle = 0;/*
        if(ImGui::DragFloat4("RotateAxis", &axis.x, 0.01f))
            model_->quaternion_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
        if (ImGui::DragFloat("RotateAngle", &angle, 0.01f))
            model_->quaternion_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);*/

        /*static Vector3 kesaS = { 0,0,0 };
        static Vector3 kesaE = { 0,0,0 };
        static float kesaSAngle = 0;
        static float kesaEAngle = 0;
        if (ImGui::DragFloat3("KesagiriStart", &kesaS.x, 0.01f))
            kesagiriStart_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
        if (ImGui::DragFloat3("KesagiriEnd", &kesaE.x, 0.01f))
            kesagiriEnd_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
        if (ImGui::DragFloat("KesagiriStartAngle", &kesaSAngle, 0.01f))
            kesagiriStart_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
        if (ImGui::DragFloat("KesagiriEndAngle", &kesaEAngle, 0.01f))
            kesagiriEnd_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);*/


        if (ImGui::Button("Set"))
        {
            modelPath_ = modelName_;
            model_->SetModel(modelPath_);
        }
        ImGui::Text("ModelPath : %s", modelPath_.c_str());
        if (ImGui::Button("Save"))
        {
            jsonBinder_->Save();
        }
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
    ImGui::End();
#endif // _DEBUG
}

bool Sword::ToTargetQuaternion(const Quaternion& _targetQuaternion, float _duration)
{
    //// 経過時間を加算
    //elapsedTime += Time::GetDeltaTime<float>();
    //// 媒介変数tを求める
    //float t = elapsedTime / _duration;
    //// クォータニオンを補間
    //model_->quaternion_ = Slerp(model_->quaternion_, _targetQuaternion, t);

    //if (t >= 1.0f)
    //{
    //    elapsedTime = 0.0f;
    //    model_->quaternion_ = _targetQuaternion;
    //    return true;
    //}
    return false;
}

//void Sword::BeginKesagiri()
//{
//    // 開始時のクォータニオンを保存
//    beginQuaternion_ = model_->quaternion_;
//
//    // セットアップ中
//    duringSetup_ = true;
//}
//
//void Sword::Kesagiri()
//{
//    if (isActionActive_)
//        return;
//
//    // セットアップ中
//    if (duringSetup_)
//    {
//        // 経過時間を加算
//        elapsedTime += Time::GetDeltaTime<float>();
//        // 媒介変数tを求める
//        float t = elapsedTime / IdleToKesaDuration_;
//        // クォータニオンを補間
//        model_->quaternion_ = Slerp(beginQuaternion_, kesagiriStart_, t);
//
//        // 経過時間が終了時間を超えたら初期化
//        if (t >= 1.0f)
//        {
//            elapsedTime = 0.0f;
//            model_->quaternion_ = kesagiriStart_;
//        }
//    }
//    else
//    {
//        // 経過時間を加算
//        elapsedTime += Time::GetDeltaTime<float>();
//        // 媒介変数tを求める
//        float t = elapsedTime / kesagiriDuration_;
//        // クォータニオンを補間
//        model_->quaternion_ = Slerp(kesagiriStart_, kesagiriEnd_, t);
//        // 経過時間が終了時間を超えたら初期化
//        if (t >= 1.0f)
//        {
//            elapsedTime = 0.0f;
//            model_->quaternion_ = kesagiriEnd_;
//        }
//    }
//}
