#pragma once
#include "D2State.h"
class CD2AndarielSprayState :
    public CD2State
{
public:
    CD2AndarielSprayState();
    CD2AndarielSprayState(const CD2AndarielSprayState& state);
    virtual ~CD2AndarielSprayState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielSprayState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimFrame();
    void OnAnimEnd();

private:
    Vector3 mSprayDir;
    Matrix mMatRot;
    std::string mAnimName;
};

