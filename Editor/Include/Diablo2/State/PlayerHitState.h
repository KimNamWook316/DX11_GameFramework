#pragma once

#include "D2State.h"

class CPlayerHitState :
    public CD2State
{
public:
    CPlayerHitState();
    CPlayerHitState(const CPlayerHitState& state);
    virtual ~CPlayerHitState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CPlayerHitState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;

public:
    void OnAnimEnd();

public:
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
};

