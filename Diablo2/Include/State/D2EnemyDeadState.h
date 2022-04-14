#pragma once

#include "D2State.h"

class CD2EnemyDeadState :
    public CD2State
{
public:
    CD2EnemyDeadState();
    CD2EnemyDeadState(const CD2EnemyDeadState& state);
    virtual ~CD2EnemyDeadState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2EnemyDeadState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnDissolveEnd();

private:
    class CDissolveComponent* mDissolve;
};

