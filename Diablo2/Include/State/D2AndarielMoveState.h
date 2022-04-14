#pragma once

#include "D2State.h"
#include "../D2Info.h"

class CD2AndarielMoveState :
    public CD2State
{
public:
    CD2AndarielMoveState();
    CD2AndarielMoveState(const CD2AndarielMoveState& state);
    virtual ~CD2AndarielMoveState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual CState* Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielMoveState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnMove();
    void OnMoveEnd();

private:
    eD2SpriteDir mPrevDir;
    float mTimer;
    float mDist;
    float mAttackTimer;
    bool mbAttack;
};

