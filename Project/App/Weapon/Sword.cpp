#include "Sword.h"

void Sword::Initialize()
{
    model_ = std::make_unique<ObjectModel>();


    jsonBinder_ = std::make_unique<JsonBinder>("Sword", "Resources/Data/Parameter/");
    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("Offset", &model_->translate_);


    if (modelPath_.empty())
        modelPath_ = "Weapon/Sword.gltf";

    model_->Initialize(modelPath_, "Sword");

    collider_ = std::make_unique<Collider>();
    collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
    collider_->SetShape(model_->GetMin(), model_->GetMax());
    collider_->SetAtrribute("Sword");
    collider_->SetMask({ "Sword","Player" });
    collider_->SetGetWorldMatrixFunc([this]() {return model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });

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
