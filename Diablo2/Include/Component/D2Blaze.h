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

public:
    void SetParent(CD2Blaze* com)
    {
        mParent = com;
    }

    void SetTileIdx(const int idx)
    {
        mTileIdx = idx;
    }

    virtual void SetSkillOwner(class CGameObject* obj) override;

public:
    int GetTileIndex()
    {
        return mTileIdx;
    }

protected:
    class CColliderBox2D* mCollider;
    class CSpriteComponent* mSprite;
    CD2Blaze* mParent;
    CD2Blaze* mChild;
    float mTimer;
    bool mbEnd;
    bool mbRegen;
    int mTileIdx;
    float mGlobalLifeTimer;
    bool mbFirst;
    bool mbInit;
};

