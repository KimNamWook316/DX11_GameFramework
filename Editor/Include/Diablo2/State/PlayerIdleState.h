#pragma once

#include "D2State.h"

class CPlayerIdleState :
    public CD2State
{
public:
    CPlayerIdleState();
    CPlayerIdleState(const CPlayerIdleState& state);
    virtual ~CPlayerIdleState();

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
    void OnClickMouseR(float deltaTime);
    void OnClickMouseL(float deltaTime);
    void OnCtrlDown(float deltaTime);
    void OnCtrlUp(float deltaTime);

private:
    bool mbRun;
    bool mbCasting;
    bool mbMelee;
};

