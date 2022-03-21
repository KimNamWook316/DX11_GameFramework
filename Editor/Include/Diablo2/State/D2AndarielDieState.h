#pragma once
#include "D2State.h"
class CD2AndarielDieState :
    public CD2State
{
public:
    CD2AndarielDieState();
    CD2AndarielDieState(const CD2AndarielDieState& state);
    virtual ~CD2AndarielDieState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual CState* Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielDieState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

private:
    float mShakeCameraTimer;
};

