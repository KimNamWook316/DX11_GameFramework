#pragma once

#include "Component/State.h"
#include "../D2Info.h"

class CPlayerRunState :
    public CState
{
public:
    CPlayerRunState();
    CPlayerRunState(const CPlayerRunState& state);
    virtual ~CPlayerRunState();

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
    void OnPushMouseR(float deltaTime);
    void OnPushMouseL(float deltaTime);
    void OnCtrlUp(float delatTime);
    void OnRunEnd();
    void OnRun();

private:
    CSharedPtr<class CD2CharacterInfoComponent> mCharInfo;
    bool mbRunEnd;
    bool mbWalk;
    eD2SpriteDir mPrevDir;
};

