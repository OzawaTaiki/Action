#pragma once
#include <Features/Scene/Interface/BaseScene.h>
#include <Features/Camera/Camera/Camera.h>
#include <Features/Camera/DebugCamera/DebugCamera.h>
#include <Features/Model/ObjectModel.h>
#include <Features/Model/AnimationModel.h>

#include <Features/Effect/Manager/ParticleManager.h>
#include <Features/LineDrawer/LineDrawer.h>
#include <System/Input/Input.h>
#include <System/Time/GameTime.h>
#include <Features/UI/UIButton.h>
#include <Features/Model/Primitive/Ring.h>
#include <Features/Model/Primitive/Ellipse.h>
#include <Features/Model/Primitive/Cylinder.h>
#include <Features/UVTransform/SpriteSheetAnimetion.h>
#include <Features/UVTransform/UVTransformAnimation.h>

#include <memory>

class SampleScene : public BaseScene
{
public:

     ~SampleScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    // シーン関連
    Camera SceneCamera_ = {};
    DebugCamera debugCamera_ = {};
    bool enableDebugCamera_ = false;

    std::vector<Particle> particles_;

    LineDrawer* lineDrawer_ = nullptr;
    Input* input_ = nullptr;
    ParticleManager* particleManager_ = nullptr;

    std::unique_ptr<AnimationModel> aModel_ = nullptr;

    std::unique_ptr<ObjectModel> oModel_= nullptr;

    Sprite* sprite_ = nullptr;

    std::unique_ptr<LightGroup> lights_;



#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};
