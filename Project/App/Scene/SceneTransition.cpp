#include "SceneTransition.h"

#include <Math/MyLib.h>
#include <Math/Easing.h>
#include <Core/DXCommon/TextureManager/TextureManager.h>

void SceneTransition::Initialize()
{
    gameTime_ = GameTime::GetInstance();
    gameTime_->CreateChannel("Transition");

    channel_ = &gameTime_->GetChannel("Transition");

    channel_->GetDeltaTime<float>();

    uint32_t textureHandle = TextureManager::GetInstance()->Load("Circle.png");
    sprite_ = Sprite::Create(textureHandle);
    sprite_->Initialize();
    sprite_->translate_ = { 640,360 };
    sprite_->SetColor({ 0,0,0,1 });
}

void SceneTransition::Start()
{
    isEnd_ = false;
    canSwitch_ = false;
    radius_ = 0.0f;
    duration_ = 0.0f;

}

void SceneTransition::Update()
{
    duration_ += channel_->GetDeltaTime<float>();

    float t = 0;
    if (duration_ != 0)
        t = duration_ / time_;

    if (canSwitch_)
    {
        t = 1 - (t - 1);
        if (t < 0)
            t = 0;
    }

    if (t >= 1.0f)
    {
        t = 1;
        canSwitch_ = true;
    }


    float easedT = Easing::EaseInOutCirc(t);

    radius_ = Lerp(0.0f, maxRadius_, easedT);

    if (radius_ >= maxRadius_)
    {
        radius_ = maxRadius_;
    }
    else if (radius_ <= 0.0f)
    {
        radius_ = 0.0f;
        isEnd_ = true;
    }

    sprite_->SetSize({ radius_, radius_  });

}

void SceneTransition::Draw()
{
    Sprite::PreDraw();
    sprite_->Draw();
}


void SceneTransition::End()
{
}
