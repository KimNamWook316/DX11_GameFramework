#pragma once

#include "D2SkillObject.h"

class CD2FrozenOrb :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2FrozenOrb();
    CD2FrozenOrb(const CD2FrozenOrb& com);
    virtual ~CD2FrozenOrb();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2FrozenOrb* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;

private:
    void fireIceBolt(const Vector3& dir);
    void explode();
    void onExplodeEnd();

private:
    float mExplodeTimer;
    float mFireTimer;
    float mFireInterval;
    float mRotationSpeed;
    CSharedPtr<class CSceneComponent> mRotation;
    class CGameObject* mIceBolt;
};

