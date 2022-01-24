#pragma once

#include "D2State.h"

class CD2PlayerRun :
    public CD2State
{
    friend class CD2Player;

public:
    virtual void Update(float deltaTime) override;
    virtual void OnEnterState(float deltaTime) override;
    virtual void OnExitState(float deltaTime) override;
    virtual void Clear() override;
    virtual CD2State* GetNextState() override;

};

