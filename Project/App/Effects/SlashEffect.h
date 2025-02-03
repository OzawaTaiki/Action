#pragma once

#include <Features/Effect/Effect/Effect.h>
#include <Features/Model/Transform/WorldTransform.h>
#include <Features/Model/Primitive/Ring.h>

class  Camera;
class SlashEffect
{
public:
    SlashEffect() = default;
    ~SlashEffect() = default;

    void Initialize();
    void Update();
    void Draw(const Camera* _camera);

    void SetActive(bool _active);
    bool IsActive() const;

    void SetParentMatrix(const WorldTransform* _parentMat);

private:

    void RingInit();
    void RingUpdate();

    void GenerateImpactWave();

    float elapsedTime_ = 0.0f;
    bool isActive_ = false;

    Effect slashEffect_;

    std::unique_ptr<Ring> ring_ = nullptr;

    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    const WorldTransform* parentMatWorld_ = nullptr;

    struct ImpactWaveParams
    {
        float duration = 0.0f;
        float delayTime = 0.0f;

        float startOuterRadius = 0.0f;
        float endOuterRadius = 0.0f;

        float startInnerRadius = 0.0f;
        float endInnerRadius = 0.0f;

        float currentScsale = .3f;
        float startScale = 0;
        float endScale = 0;

        Vector4 startColor = { 1,1,1,1 };
        Vector4 endColor = { 1,1,1,0 };
        Vector4 currentColor = { 1,1,1,1 };

        uint32_t divide = 0;

        uint32_t textureHandle = 0;

        bool isDraw = false;
    };

    ImpactWaveParams impactWaveParams_;
};
