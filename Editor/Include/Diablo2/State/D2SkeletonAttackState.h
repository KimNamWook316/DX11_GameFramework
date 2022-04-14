#pragma once
#include "D2State.h"
class CD2SkeletonAttackState :
    public CD2State
{
public:
    CD2SkeletonAttackState();
    CD2SkeletonAttackState(const CD2SkeletonAttackState& state);
    virtual ~CD2SkeletonAttackState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2SkeletonAttackState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();

private:
    float mDist;
    bool mbOutRange;
    bool mbAnimEnd;
    class CGameObject* mAttackObj;
};

