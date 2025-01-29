#include "Enemy.h"

uint32_t Enemy::enemyCount_ = 0;

Enemy::Enemy()
{
    enemyID_ = enemyCount_++;
}

void Enemy::Initialize()
{
    isAlive_ = true;

    std::string enemyName = "Enemy";
    jsonBinder_ = std::make_unique<JsonBinder>(enemyName, "Resources/Data/Parameter/");

    jsonBinder_->RegisterVariable("hp", &maxHP_);
    jsonBinder_->RegisterVariable("color", &color_);
    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("MoveSpeed", &moveSpeed_);
    jsonBinder_->RegisterVariable("ChaseEndDistance", &chaseEndDistance_);

    hp_ = maxHP_;

    if (modelPath_.empty())
        modelPath_ = "Enemy/Enemy.gltf";

    model_ = std::make_unique<ObjectModel>();
    model_->Initialize(modelPath_, enemyName);

    InitCollider();

    f_currentState_ = [this]() {ChasePlayer(); };

    circleShadow_ = std::make_unique<CircleShadow>();
    circleShadow_->Initialize(model_->GetWorldTransform());
    circleShadow_->SetScale({ .5f,0.00001f,.5f });

    HPUI_ = std::make_unique<EnemyHPUI>();
    HPUI_->Initialize(maxHP_, { 1,0,0,1 }, { 0,0,0,1 }, model_->GetWorldTransform(), &hp_);


}

void Enemy::Update()
{
#ifdef _DEBUG
    //ImGui();
#endif // _DEBUG

    color_ = { 1,1,1,1 };

#ifdef _DEBUG
    if (idleFlag_)          Idle();
    else                    f_currentState_();
#else
    f_currentState_();
#endif

    if(isAlive_)
        collider_->RegsterCollider();

    model_->Update();
}

void Enemy::Draw(const Camera* _camera)
{
    model_->Draw(_camera, color_);

    circleShadow_->Draw(_camera);


#ifdef _DEBUG
    if (gui_drawCollider_)
        collider_->Draw();
#endif // _DEBUG
}

void Enemy::DrawFront(const Camera* _camera)
{
    HPUI_->Draw(_camera);
}

void Enemy::ImGui()
{
#ifdef _DEBUG
    std::string name = "Enemy_" + std::to_string(enemyID_);

    ImGui::Begin("Enemy");
    ImGui::BeginTabBar("Enemy");
    if (ImGui::BeginTabItem(name.c_str()))
    {
        ImGui::Text("HP : %f", hp_);
        ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f, 0.01f);
        ImGui::DragFloat("ChaseEndDistance", &chaseEndDistance_, 0.01f, 0.01f);
        ImGui::ColorEdit4("Color", &color_.x);
        ImGui::Checkbox("Idle", &idleFlag_);
        ImGui::Checkbox("DrawCollider", &gui_drawCollider_);
        ImGui::InputText("ModelPath", modelName_, 256);
        if (ImGui::Button("Set"))
        {
            modelPath_ = modelName_;
            model_->SetModel(modelPath_);
        }
        ImGui::Text("ModelPath : %s", modelPath_.c_str());

        if (ImGui::Button("Delete"))
        {
            isAlive_ = false;
        }

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

void Enemy::OnCollision(const Collider* _other)
{
    if (_other->GetName() == "Sword")
    {
        hp_ -= 1;
        color_ = { 1,0,0,1 };
        if (hp_ <= 0)
        {
            isAlive_ = false;
        }
    }
    else if (_other->GetName() == "Enemy")
    {
        uint32_t id = std::atoi(_other->GetId().c_str());
        // 自分よりIDが小さい敵にぶつかったら無視
        if (id < enemyID_)
            return;

        // ぶつかったら離れる
        Vector3 direction = _other->GetWorldMatrix().GetTranslate() - model_->GetWorldTransform()->GetWorldPosition();
        direction.y = 0;
        model_->translate_ -= direction.Normalize() * moveSpeed_;
    }
}

void Enemy::InitCollider()
{
    collider_ = std::make_unique<Collider>();
    collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
    collider_->SetShape(model_->GetMin(), model_->GetMax());
    collider_->SetAtrribute("Enemy");
    collider_->SetMask({ "Player" });
    collider_->SetGetWorldMatrixFunc([this]() {return model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });
    collider_->SetReferencePoint({ 0,0,0 });
    collider_->SetId(std::to_string(enemyID_));
}

void Enemy::Idle()
{
    // 何もしない

    assert(playerWT_ && "PlayerWorldTransform is nullptr");

   // 距離が離れたら追いかける
    Vector3 playerPosition = playerWT_->GetWorldPosition();
    Vector3 enemyPosition = model_->GetWorldTransform()->GetWorldPosition();
    Vector3 direction = playerPosition - enemyPosition;

    if (direction.Length() > chaseEndDistance_)
    {
        f_currentState_ = [this]() {ChasePlayer(); };
    }
}

void Enemy::ChasePlayer()
{
    assert(playerWT_ && "PlayerWorldTransform is nullptr");

    Vector3 playerPosition = playerWT_->GetWorldPosition();
    Vector3 direction = playerPosition - model_->GetWorldTransform()->GetWorldPosition();;
    direction.y = 0;
    model_->translate_ += direction.Normalize() * moveSpeed_;

    Rotation();

    if (direction.Length() < chaseEndDistance_)
    {
        f_currentState_ = [this]() {Idle(); };
    }
}

void Enemy::Rotation()
{
    Vector3 playerPosition = playerWT_->GetWorldPosition();
    Vector3 direction = playerPosition - model_->GetWorldTransform()->GetWorldPosition();;
    direction.y = 0;
    float angle = atan2(direction.x, direction.z);
    model_->euler_.y = angle;
}
