#pragma once
#include "D2State.h"
class CD2EnemyMeleeAttackState :
    public CD2State
{
public:
    CD2EnemyMeleeAttackState();
    CD2EnemyMeleeAttackState(const CD2EnemyMeleeAttackState& state);
    virtual ~CD2EnemyMeleeAttackState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2EnemyMeleeAttackState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();

private:
    bool mbKeepAttack;
    bool mbAnimEnd;
    bool mbHit;
};

