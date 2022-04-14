#pragma once
#include "D2State.h"
class CD2AndarielMeleeAttackState :
    public CD2State
{
public:
    CD2AndarielMeleeAttackState();
    CD2AndarielMeleeAttackState(const CD2AndarielMeleeAttackState& state);
    virtual ~CD2AndarielMeleeAttackState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielMeleeAttackState* Clone() override;
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
    bool mbSpray;
    bool mbAnimEnd;
    class CGameObject* mAttackObj;
};

