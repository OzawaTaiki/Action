#pragma once

#include "BaseEnemy.h"
#include <Systems/>

class Enemy : public BaseEnemy
{
public:
    Enemy() = default;
    ~Enemy() = default;

    void Initialize() override;
    void Update() override;
    void Draw(const Camera* _camera) override;

    void ImGui() override;

private:



};