#include "SlashEffect.h"

#include <Math/Random/RandomGenerator.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>

#include <numbers>

void SlashEffect::Initialize()
{

    slashEffect_.Initialize("SlashEffect_01");

    slashEffect_.SetTimeChannel("Effects");

    RingInit();

}

void SlashEffect::Update()
{

    if (!isActive_)
        return;


    slashEffect_.Update();
    this->RingUpdate();

}

void SlashEffect::Draw(const Camera* _camera)
{
    slashEffect_.Draw();
    ring_->Draw(*_camera, impactWaveParams_.currentColor);
}

void SlashEffect::SetActive(bool _active)
{
    if (isActive_)
        return;

    isActive_ = _active;
    slashEffect_.SetActive(_active);
    impactWaveParams_.isDraw = _active;

    if (_active)
    {
        elapsedTime_ = 0.0f;
        GenerateImpactWave();
    }
}

bool SlashEffect::IsActive() const
{
    return isActive_;
}

void SlashEffect::SetParentMatrix(const WorldTransform* _parentMat)
{
    slashEffect_.SetParentMatrix(&_parentMat->matWorld_);

    parentMatWorld_ = _parentMat;
}

void SlashEffect::RingInit()
{
    ring_ = std::make_unique<Ring>();
    ring_->SetInnerRadius(0.5f);
    ring_->SetOuterRadius(1.0f);
    ring_->SetDivide(32);
    ring_->SetBillboard({ true,true,true });
    ring_->SetTexture("particle/circle.png");
    ring_->Generate();
    ring_->SetUseQuaternion(false);
    ring_->SetTexture(0);

    impactWaveParams_.duration = 1.0f;
    impactWaveParams_.delayTime = 0.0f;
    impactWaveParams_.startOuterRadius = 0.5f;
    impactWaveParams_.endOuterRadius = 1.0f;
    impactWaveParams_.startInnerRadius = 0.0f;
    impactWaveParams_.endInnerRadius = 0.5f;
    impactWaveParams_.startColor = { 1,1,1,1 };
    impactWaveParams_.endColor = { 1,1,1,0 };
    impactWaveParams_.divide = 32;
    impactWaveParams_.textureHandle = TextureManager::GetInstance()->Load("whiteAlpha.png");
    impactWaveParams_.isDraw = false;
    ring_->SetTexture(impactWaveParams_.textureHandle);

}

void SlashEffect::RingUpdate()
{
    float deltaTime = GameTime::GetChannel("default").GetDeltaTime<float>();

    elapsedTime_ += deltaTime;


    float t = elapsedTime_ / impactWaveParams_.duration;
    if (t >= 1.0f)
    {
        t = 1.0f;

        impactWaveParams_.isDraw = false;
        isActive_ = false;
    }

    float scaleEeasedT = Easing::EaseOutExpo(t);
    float colorEasedT = Easing::EaseOutCubic(t);

   /* float outerRadius = Lerp(impactWaveParams_.startOuterRadius, impactWaveParams_.endOuterRadius, easedT);
    float innerRadius = Lerp(impactWaveParams_.startInnerRadius, impactWaveParams_.endInnerRadius, easedT);
    ring_->SetOuterRadius(outerRadius);
    ring_->SetInnerRadius(innerRadius);*/


    Vector4 color = Lerp(impactWaveParams_.startColor, impactWaveParams_.endColor, colorEasedT);
    float scale = Lerp(impactWaveParams_.startScale, impactWaveParams_.endScale, scaleEeasedT);

    impactWaveParams_.currentColor = color;

    ring_->scale_ = scale;

    if (colorEasedT >= 0.85f)
        impactWaveParams_.currentColor = { 1,0,0,0 };

    ring_->Update();


}

void SlashEffect::GenerateImpactWave()
{
    float PI = std::numbers::pi_v<float>;
    Vector3 rot = RandomGenerator::GetInstance()->GetUniformVec3({ -PI,-PI ,-PI }, { PI,PI ,PI });
    //ring_->rotation_ = rot;
    ring_->rotation_.y = 0;

    Vector3 offset = { 0,1.f,0 };
    Matrix4x4 rotMat = MakeRotateMatrix(parentMatWorld_->rotate_);
    ring_->translate_ = Transform(offset, parentMatWorld_->matWorld_);

    impactWaveParams_.startScale = .5f;
    impactWaveParams_.endScale = 1.2f;

    impactWaveParams_.startColor = { 1,0,0,1 };
    impactWaveParams_.endColor = { 1,0,0,0 };
}
