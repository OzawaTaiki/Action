#include "Sword.h"
#include <numbers>
#include <Math/MyLib.h>
#include <System/Time/Time.h>
#include <Math/Easing.h>


void Sword::Initialize()
{
    model_ = std::make_unique<AnimationModel>();


    jsonBinder_ = std::make_unique<JsonBinder>("Sword", "Resources/Data/Parameter/");
    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("Offset", &model_->translate_);



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

    model_->quaternion_ = Quaternion::MakeRotateAxisAngleQuaternion({ 1,0,0 }, 1.57f);

    for (auto& effect : slashEffect_)
    {
        effect = std::make_unique<SlashEffect>();
        effect->Initialize();
        effect->SetParentMatrix(model_->GetWorldTransform());
    }

}

void Sword::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG

    //collider_->RegsterCollider();

    for (size_t i = 0; i < slashEffect_.size(); ++i)
    {
        if (isSlashEffectActive_[i])
            slashEffect_[i]->Update();

        if (!slashEffect_[i]->IsActive())
        {
            isSlashEffectActive_[i] = false;
        }
    }

    model_->Update();
}

void Sword::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });

    for (auto& effect : slashEffect_)
    {
        effect->Draw(_camera);
    }

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

        model_->quaternion_ = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);

        if (ImGui::TreeNode("SlashEffects"))
        {
            int count = 0;
            for (auto& effect : slashEffect_)
            {
                ImGui::Text("SlashEffect_%d : %s", count++, effect->IsActive() ? "Active" : "inactive");
            }
            ImGui::TreePop();
        }


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


    if (ImGui::Button("SlashEffect"))
    {
        slashEffect_[0]->SetActive(1);
        isSlashEffectActive_[0] = true;
    }
    static bool imLoop = false;
    ImGui::Checkbox("Loop", &imLoop);
    if (imLoop)
    {
        if (!isSlashEffectActive_[0])
        {
            slashEffect_[0]->SetActive(1);
            isSlashEffectActive_[0] = true;
        }
    }
    if(camera_)
        camera_->ShakeParametaerSettingFromImGui();

    ImGui::End();
#endif // _DEBUG
}

void Sword::OnCollision(const Collider* _other)
{
    if (_other->GetName() == "Enemy")
    {
        if (collider_->IsCollisionEnter())
        {
            for (size_t i = 0; i < slashEffect_.size(); ++i)
            {
                if (!isSlashEffectActive_[i])
                {
                    slashEffect_[i]->SetActive(true);
                    isSlashEffectActive_[i] = true;
                    if (*currentCombo_ != std::nullopt && *currentCombo_ == 2)
                    {
                        GameTime::GetChannel("default").ApplyHitStop(0.2f);
                        camera_->Shake(0.15f, { -0.05f,-0.05f }, { 0.05f,0.05f });
                    }
                    break;
                }
            }
        }
    }
}

void Sword::RegsitCollider()
{
    collider_->RegsterCollider();

#ifdef _DEBUG
    isDrawCollider_ = true;
#endif // _DEBUG

}
