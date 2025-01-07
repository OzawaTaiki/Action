#include "Sword.h"
#include <numbers>
#include <Physics/Math/MyLib.h>
#include <Systems/Time/Time.h>
#include <Physics/Math/Easing.h>


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
    collider_->SetGetWorldMatrixFunc([this]() {return  model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });

    model_->rotate_ = Quaternion::MakeRotateAxisAngleQuaternion({ 1,0,0 }, 1.57f);


    //Vector3 axis = Vector3{ -1,1,0 }.Normalize();
    //attackParams_["attack_01"] = {
    //    Quaternion::MakeRotateAxisAngleQuaternion(axis, 0),
    //    Quaternion::MakeRotateAxisAngleQuaternion(axis, std::numbers::pi_v<float>),
    //    0.4f,
    //    0.15f,
    //    0.2f,
    //    static_cast<uint32_t>(Easing::EasingFunc::EaseOutExpo)
    //};

    //Quaternion q = attackParams_["attack_01"].endQuaternion;
    //attackParams_["attack_02"] = {
    //    q * Quaternion::MakeRotateAxisAngleQuaternion({1,0,0}, 0),
    //    q * Quaternion::MakeRotateAxisAngleQuaternion({1,0,0}, std::numbers::pi_v<float>),
    //    0.4f,
    //    0.15f,
    //    0.2f,
    //    static_cast<uint32_t>(Easing::EasingFunc::EaseOutExpo)
    //};

    //Quaternion q2 = attackParams_["attack_02"].endQuaternion;
    //attackParams_["attack_03"] = {
    //    q2 * Quaternion::MakeRotateAxisAngleQuaternion({ 0,1,1 }, 0),
    //    q2 * Quaternion::MakeRotateAxisAngleQuaternion({ 0,1,1 }, std::numbers::pi_v<float>),
    //    0.4f,
    //    0.15f,
    //    0.2f,
    //    static_cast<uint32_t>(Easing::EasingFunc::EaseOutExpo)
    //};

}

void Sword::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG

    //if (isActionActive_)
    //{
    //    elapsedTime += Time::GetDeltaTime<float>();
    //    float t = elapsedTime / attackParams_[currentAction_].duration;
    //    float easedT = Easing::SelectFuncPtr(attackParams_[currentAction_].easingType)(t);
    //    auto param = attackParams_[currentAction_];
    //    model_->rotate_ = Slerp(param.startQuaternion, param.endQuaternion, easedT);
    //    if (t >= 1.0f)
    //    {
    //        elapsedTime = 0.0f;
    //        isActionActive_ = false;
    //        isWaitingForIdle_ = true;
    //    }
    //
    //    model_->rotate_ = model_->rotate_.Normalize();
    //}
    //else if (!isIdle_)
    //{
    //    //elapsedTime += Time::GetDeltaTime<float>();
    //    elapsedTime += 1.0f / 60.0f;
    //    if (elapsedTime > attackParams_[currentAction_].timeToIdle ||
    //        !isActionActive_ && !isWaitingForIdle_
    //        )
    //    {
    //        if(isWaitingForIdle_)
    //        {
    //            isWaitingForIdle_ = false;
    //            elapsedTime = 0.0f;
    //        }
    //
    //        auto param = attackParams_[currentAction_];
    //        float t = elapsedTime / attackParams_[currentAction_].toIdleDuration;
    //        model_->rotate_ = Slerp(param.endQuaternion, Quaternion::Identity(), t);
    //
    //        if (t >= 1.0f)
    //        {
    //            elapsedTime = 0.0f;
    //            isIdle_ = true;
    //            model_->rotate_ = Quaternion::Identity();
    //        }
    //    }
    //}

    //collider_->RegsterCollider();

    model_->Update();
}

void Sword::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });

#ifdef _DEBUG
    if (isDrawCollider_||gui_drawCollider_)
    {
        collider_->Draw();
        isDrawCollider_ = false;
    }
#endif // _DEBUG
}

void Sword::ImGui()
{
#ifdef _DEBUG
    ImGui::Begin("Player");
    ImGui::BeginTabBar("Player");
    if (ImGui::BeginTabItem("Sword"))
    {
        ImGui::Checkbox("DrawCollider", &gui_drawCollider_);
        ImGui::InputText("ModelPath", modelName_, 256);
        ImGui::DragFloat3("Offset", &model_->translate_.x, 0.01f);
        static Vector3 axis = { 0,1,0 };
        static float angle = 0;
        ImGui::DragFloat3("RotateAxis", &axis.x, 0.01f);
        ImGui::DragFloat("RotateAngle", &angle, 0.01f);

        model_->rotate_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);

        if (ImGui::Button("kesagiri"))
        {
            model_->SetAnimation("kesagiri", false);
        }

        if (ImGui::Button("Init"))
            Initialize();

        /*
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
            kesagiriEnd_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
            */


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

void Sword::RegsitCollider()
{
    collider_->RegsterCollider();
#ifdef _DEBUG
    isDrawCollider_ = true;
#endif // _DEBUG

}
