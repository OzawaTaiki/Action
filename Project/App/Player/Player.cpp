#include "Player.h"
#include <Systems/Input/Input.h>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/VectorFunction.h>
#include <Physics/Math/MyLib.h>


void Player::Initialize()
{
    jsonBinder_ = std::make_unique<JsonBinder>("Player", "Resources/Data/Parameter/");

    jsonBinder_->RegisterVariable("ModelPath", &modelPath_);
    jsonBinder_->RegisterVariable("MoveSpeed", &moveSpeed_);

    if (modelPath_.empty())
        modelPath_ = "Sphere/Sphere.obj";

    model_ = std::make_unique<AnimationModel>();
    model_->Initialize(modelPath_);

    collider_ = std::make_unique<Collider>();
    collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
    collider_->SetShape(model_->GetMin(), model_->GetMax());
    collider_->SetAtrribute("Player");
    collider_->SetMask("Player");
    collider_->SetGetWorldMatrixFunc([this]() {return model_->GetWorldTransform()->matWorld_; });
    collider_->SetOnCollisionFunc([this](const Collider* _other) {OnCollision(_other); });
    collider_->SetReferencePoint({ 0,0,0 });

    weapon_ = std::make_unique<Sword>();
    weapon_->Initialize();
    //weapon_->SetParent(model_->GetWorldTransform());

    f_currentState_ = std::bind(&Player::Idle, this);

    currentAttack_ = std::nullopt;


    // 右手のトランスフォームを別で用意
    rightArm_.Initialize();
    // それを武器の親にする
    weapon_->SetParent(&rightArm_);

}

void Player::Update(const Vector3& _cameraroate)
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG


    Matrix4x4 cameraRotMat = MakeRotateMatrix(_cameraroate);
    Move(cameraRotMat);

    f_currentState_();

    Rotation();

    collider_->RegsterCollider();

    model_->Update();
    weapon_->Update();

    UpdateRightArmTrans();
}

void Player::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });

    weapon_->Draw(_camera);
#ifdef _DEBUG
    if (gui_drawCollider_)
        collider_->Draw();
#endif // _DEBUG

}

void Player::UpdateRightArmTrans()
{
    rightArm_.matWorld_ = model_->GetSkeletonSpaceMatrix(3);
    rightArm_.matWorld_ *= model_->GetWorldTransform()->matWorld_;
}

void Player::Move(const Matrix4x4& _cameraRotMat)
{
    Input::GetInstance()->GetMove(move_, moveSpeed_);

    // 移動がない場合コントローラー
    if (move_ == Vector3(0, 0, 0))
    {
        Vector2 padMove = Input::GetInstance()->GetPadLeftStick();
        move_.x = padMove.x;
        move_.z = padMove.y;

        move_ *= moveSpeed_;
    }

    move_.y = 0;
    Vector3 normalizeMove = move_.Normalize();

    move_ = TransformNormal(move_, _cameraRotMat);

    model_->translate_ += move_;

    if (normalizeMove != Vector3(0, 0, 0))
    {
        targetDirection_ = normalizeMove;
        targetQuaternion_ = Quaternion::FromToRotation(Vector3(0, 0, 1), targetDirection_);
    }
}

// TODO : Playerの攻撃（とりあえず単発） 敵にダメージを入れられるように。敵HP0以下で削除
// 右上から左下への袈裟斬り qauternionで回転させると楽だと思うが...
// TODO : 敵の攻撃（とりあえず単発） Playerにダメージを入れられるように。PlayerHP0以下でゲームオーバー

void Player::Rotation()
{
    model_->rotate_ = Slerp(model_->rotate_, targetQuaternion_, 0.1f);
}

void Player::Attack()
{
    // アクションが終了したらIdle状態に戻る
    if ((model_->IsAnimationEnd() || toIdle_) && !isTrigger_)
    {
        model_->ToIdle(1.0f);
        f_currentState_ = std::bind(&Player::Idle, this);
        currentAttack_ = std::nullopt;
        isAttacking_ = false;
        toIdle_ = false;
        return;
    }

    // 入力直後の処理
    if (isTrigger_)
    {
        if (currentAttack_ == std::nullopt)
        {
            currentAttack_ = 0;
            model_->SetAnimation(attackNames_[*currentAttack_]);
            isTrigger_ = false;
        }
        else if (canCommbo_)
        {
            isTrigger_ = false;
            ++(*currentAttack_);
            if (*currentAttack_ >= attackNames_.size())
            {
                // コンボ最大までいったら攻撃終わり
                // 攻撃フラグをおろし，Idle遷移フラグを立てる
                currentAttack_ = std::nullopt;
                isAttacking_ = false;
                toIdle_ = true;
            }
            else
                model_->SetAnimation(attackNames_[*currentAttack_]);
        }
    }

    // 攻撃中にトリガーでコンボ
    if (isAttacking_)
    {
        if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A) ||
            Input::GetInstance()->IsKeyTriggered(DIK_SPACE))
        {
            isTrigger_ = true;
        }
    }

    // canCommboを立てる
    canCommbo_ = weapon_->CanCommbo();

    weapon_->RegsitCollider();


}

void Player::Idle()
{
    currentAttack_ = std::nullopt;

    if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A) ||
        Input::GetInstance()->IsKeyTriggered(DIK_SPACE))
    {
        if(canAttack_)
        {
            f_currentState_ = std::bind(&Player::Attack, this);
            isTrigger_ = true;
            isAttacking_ = true;
            return;
        }
    }
}

#ifdef _DEBUG
#include <imgui.h>
void Player::ImGui()
{
    ImGui::Begin("Player");
    ImGui::BeginTabBar("Player");
    if (ImGui::BeginTabItem("Player"))
    {
        ImGui::Text("MoveSpeed");
        ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
        if (ImGui::Checkbox("DrawCollider", &gui_drawCollider_))
            weapon_->SetDrawCollider(gui_drawCollider_);

        static char modelName[256];
        ImGui::InputText("ModelPath", modelName, 256);
        if (ImGui::Button("Set"))
        {
            modelPath_ = modelName;
            model_->SetModel(modelPath_);
        }
        ImGui::Text("ModelPath : %s", modelPath_.c_str());

        if (ImGui::Button("Attack_01"))
        {
            model_->SetAnimation("Attack_01", false);
        }
        if (ImGui::Button("Attack_02"))
        {
            model_->SetAnimation("Attack_02", false);
        }
        if (ImGui::Button("Attack_03"))
        {
            model_->SetAnimation("Attack_03", false);
        }


        if (ImGui::Button("Save"))
            jsonBinder_->Save();
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Debug"))
    {
        ImGui::Text("isTrigger : %s", isTrigger_ ? "true" : "false");
        ImGui::Text("isAttacking : %s", isAttacking_ ? "true" : "false");
        ImGui::Text("canCommbo : %s", canCommbo_ ? "true" : "false");
        ImGui::Text("canAttack : %s", canAttack_ ? "true" : "false");
        ImGui::Text("toIdle : %s", toIdle_ ? "true" : "false");
        ImGui::Text("currentAttack : %d", currentAttack_.value_or(-1));
        ImGui::Text("nextAttack : %d", nextAttack_.value_or(-1));


        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();

    ImGui::End();

}
#endif // _DEBUG
