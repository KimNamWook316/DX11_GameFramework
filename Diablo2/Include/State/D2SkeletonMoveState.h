#pragma once

#include "D2State.h"
#include "../D2Info.h"

class CD2SkeletonMoveState :
    public CD2State
{
public:
    CD2SkeletonMoveState();
    CD2SkeletonMoveState(const CD2SkeletonMoveState& state);
    virtual ~CD2SkeletonMoveState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual CState* Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2SkeletonMoveState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnMove();

private:
    eD2SpriteDir mPrevDir;
    float mDist;
    bool mbOutRange;
    float mTimer;
    bool mbAttack;
};

