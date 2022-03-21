#pragma once

#include "D2State.h"

class CD2AndarielDeadState :
    public CD2State
{
public:
    CD2AndarielDeadState();
    CD2AndarielDeadState(const CD2AndarielDeadState& state);
    virtual ~CD2AndarielDeadState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielDeadState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();
};

