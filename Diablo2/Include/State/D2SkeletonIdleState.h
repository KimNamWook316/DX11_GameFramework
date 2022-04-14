#pragma once
#include "D2State.h"
class CD2SkeletonIdleState :
    public CD2State
{
public:
    CD2SkeletonIdleState();
    CD2SkeletonIdleState(const CD2SkeletonIdleState& state);
    virtual ~CD2SkeletonIdleState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2SkeletonIdleState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

private:
    // TODO : 트리거 기반으로 바꿔야 함
    float mDist;

    bool mbAttack;
    bool mbWaiting;
};

