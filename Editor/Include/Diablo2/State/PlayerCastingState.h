#pragma once

#include "Component/State.h"

class CPlayerCastingState :
    public CState
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
    void OnAnimEnd();
};

