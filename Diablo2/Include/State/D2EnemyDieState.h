#pragma once

#include "D2State.h"

class CD2EnemyDieState :
    public CD2State
{
public:
    CD2EnemyDieState();
    CD2EnemyDieState(const CD2EnemyDieState& state);
    virtual ~CD2EnemyDieState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2EnemyDieState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

public:
    void OnAnimEnd();

private:
    bool mbAnimEnd;
};

