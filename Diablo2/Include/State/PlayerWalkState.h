#pragma once

#include "D2State.h"
#include "../D2Info.h"

class CPlayerWalkState :
    public CD2State
{
public:
    CPlayerWalkState();
    CPlayerWalkState(const CPlayerWalkState& state);
    virtual ~CPlayerWalkState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CPlayerWalkState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void ResetState() override;

public:
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;

public:
    void OnPushMouseR(float deltaTime);
    void OnPushMouseL(float deltaTime);
    void OnCtrlDown(float deltaTime);
    void OnCtrlUp(float deltaTime);
    void OnWalkEnd();
    void OnWalk();

private:
    eD2SpriteDir mPrevDir;
    bool mbRun;
    bool mbWalkEnd;
    bool mbCasting;
};

