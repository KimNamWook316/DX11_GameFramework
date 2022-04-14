#pragma once

#include "D2State.h"

class CD2EnemyIdleState :
    public CD2State
{
public:
    CD2EnemyIdleState();
    CD2EnemyIdleState(const CD2EnemyIdleState& state);
    virtual ~CD2EnemyIdleState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2EnemyIdleState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

private:
    // TODO : 트리거 기반으로 바꿔야 함
    float mDist;

    bool mbMove;
    bool mbAttack;
    bool mbWaiting;
};

