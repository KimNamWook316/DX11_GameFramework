#pragma once

#include "D2State.h"

class CD2SkeletonHitState :
    public CD2State
{
public:
    CD2SkeletonHitState();
    CD2SkeletonHitState(const CD2SkeletonHitState& state);
    virtual ~CD2SkeletonHitState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2SkeletonHitState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();

private:
    std::string mAnimName;
    float mPrevSpeed;
};

