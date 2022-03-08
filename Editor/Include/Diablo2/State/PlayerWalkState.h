#pragma once

#include "Component/State.h"
#include "../D2Info.h"

class CPlayerWalkState :
    public CState
{
public:
    CPlayerWalkState();
    CPlayerWalkState(const CPlayerWalkState& state);
    virtual ~CPlayerWalkState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CPlayerWalkState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void ResetState() override;

public:
    void OnPushMouseR(float deltaTime);
    void OnPushMouseL(float deltaTime);
    void OnCtrlDown(float deltaTime);
    void OnWalkEnd();
    void OnWalk();

private:
    eD2SpriteDir mPrevDir;
    bool mbRun;
    bool mbWalkEnd;
    CSharedPtr<class CD2CharacterInfoComponent> mCharInfo;
};

