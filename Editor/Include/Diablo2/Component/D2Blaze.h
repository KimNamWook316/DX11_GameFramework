#pragma once

#include "D2SkillObject.h"

class CD2Blaze :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2Blaze();
    CD2Blaze(const CD2Blaze& com);
    virtual ~CD2Blaze();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2Blaze* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;

public:
    void OnStartAnimEnd();
    void OnEndAnimEnd();

protected:
    CSharedPtr<class CColliderBox2D> mCollider;
    CSharedPtr<class CSpriteComponent> mSprite;
    float mTimer;
    bool mbEnd;
    float mRegenDist;
    bool mbRegen;
};

