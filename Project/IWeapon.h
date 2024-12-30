#pragma once

#include <Physics/Collision/Collider.h>

class IWeapon
{
public:

    IWeapon() = default;
    virtual ~IWeapon() = default;
    virtual void Initialize()=0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void OnCollision(const Collider* _other) = 0;
};