#pragma once
#include "D2State.h"
class CD2AndarielProjectileState :
    public CD2State
{
public:
    CD2AndarielProjectileState();
    CD2AndarielProjectileState(const CD2AndarielProjectileState& state);
    virtual ~CD2AndarielProjectileState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielProjectileState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();

private:
    Vector2 mDir;
};

