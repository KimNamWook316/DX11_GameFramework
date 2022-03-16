#pragma once

#include "D2SkillObject.h"

class CD2EnemyMeleeAttack :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2EnemyMeleeAttack();
    CD2EnemyMeleeAttack(const CD2EnemyMeleeAttack& com);
    virtual ~CD2EnemyMeleeAttack();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual CD2EnemyMeleeAttack* Clone() override;

public:
    virtual void OnCollideEnter(const CollisionResult& result);
    void OnCollideExit(const CollisionResult& result);

private:
    CSharedPtr<class CColliderBox2D> mCollider;
};

