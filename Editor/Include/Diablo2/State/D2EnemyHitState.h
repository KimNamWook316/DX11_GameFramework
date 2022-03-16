#pragma once
#include "D2State.h"
class CD2EnemyHitState :
    public CD2State
{
public:
    CD2EnemyHitState();
    CD2EnemyHitState(const CD2EnemyHitState& state);
    virtual ~CD2EnemyHitState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2EnemyHitState* Clone() override;
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

