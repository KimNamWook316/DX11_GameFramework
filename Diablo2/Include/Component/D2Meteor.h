#pragma once

#include "D2SkillObject.h"

class CD2Meteor :
	public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2Meteor();
    CD2Meteor(const CD2Meteor& com);
    virtual ~CD2Meteor();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2Meteor* Clone() override;

    virtual void OnCollideEnter(const CollisionResult& result) override;

private:
    void spreadFire();

private:
    class CColliderCircle* mCollider;
    bool mbEnd;
};

