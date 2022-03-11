#pragma once

#include "D2SkillObject.h"

class CD2MeleeAttack :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2MeleeAttack();
    CD2MeleeAttack(const CD2MeleeAttack& com);
    virtual ~CD2MeleeAttack();

public:
    void OnAttackAnimEnd();

public:
    virtual CObjectComponent* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;
};

