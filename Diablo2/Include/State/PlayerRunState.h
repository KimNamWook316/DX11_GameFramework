#pragma once

#include "D2State.h"
#include "../D2Info.h"

class CPlayerRunState :
    public CD2State
{
public:
    CPlayerRunState();
    CPlayerRunState(const CPlayerRunState& state);
    virtual ~CPlayerRunState();

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
    void OnDownMouseRCtrl(float deltaTime);
    void OnPushMouseL(float deltaTime);
    void OnCtrlUp(float delatTime);
    void OnRunEnd();
    void OnRun();

private:
    bool mbRunEnd;
    bool mbWalk;
    bool mbCasting;
    eD2SpriteDir mPrevDir;
};

