#pragma once

#include "D2State.h"

class CD2PlayerRun final :
    public CD2State
{
    friend class CD2Player;

public:
    CD2PlayerRun();
    virtual ~CD2PlayerRun();

public:
    virtual void Update(float deltaTime) override;
    virtual void OnEnterState(float deltaTime) override;
    virtual void OnExitState(float deltaTime) override;
    virtual void Clear() override;
    virtual CD2State* GetNextState() override;

private:
    class CInputStackComponent* mInputStack;
    Vector2 mTargetPos;
};

