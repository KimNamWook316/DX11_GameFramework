#pragma once

#include "Component/State.h"

class CPlayerIdleState :
    public CState
{
public:
    CPlayerIdleState();
    CPlayerIdleState(const CPlayerIdleState& state);
    virtual ~CPlayerIdleState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void ResetState() override;

public:
    void OnClickMouseR(float deltaTime);
    void OnClickMouseL(float deltaTime);
    void OnCtrlDown(float deltaTime);
    void OnCtrlUp(float deltaTime);

private:
    bool mbRun;
    CSharedPtr<class CD2CharacterInfoComponent> mCharInfo;
};

