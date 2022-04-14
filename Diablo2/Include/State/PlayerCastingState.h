#pragma once

#include "D2State.h"

class CPlayerCastingState :
    public CD2State
{
public:
    CPlayerCastingState();
    CPlayerCastingState(const CPlayerCastingState& state);
    virtual ~CPlayerCastingState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void ResetState() override;

public:
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;

public:
    void OnAnimEnd();

private:
    Vector3 mOriginSpriteScale;
};

