#pragma once
#include "D2State.h"
class CD2AndarielIdleState :
    public CD2State
{
public:
    CD2AndarielIdleState();
    CD2AndarielIdleState(const CD2AndarielIdleState& state);
    virtual ~CD2AndarielIdleState();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2AndarielIdleState* Clone() override;
    virtual void EnterStateFunction() override;
    virtual CState* StateFunction() override;
    virtual void ExitStateFunction() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
    virtual void OnCollideExit(const CollisionResult& result) override;
    virtual void ResetState() override;

private:
    // TODO : Ʈ���� ������� �ٲ�� ��
    float mDist;

    bool mbMove;
    bool mbAttack;
    bool mbWaiting;
    bool mbHit;
};

