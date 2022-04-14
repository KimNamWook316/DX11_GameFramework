#pragma once
#include "D2SkillObject.h"
class CD2MeteorFire :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2MeteorFire();
    CD2MeteorFire(const CD2MeteorFire& com);
    virtual ~CD2MeteorFire();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2MeteorFire* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;

public:
    void OnStartAnimEnd();
    void OnEndAnimEnd();

private:
    class CColliderBox2D* mCollider;
    class CSpriteComponent* mSprite;
    float mTimer;
    bool mbEnd;
};

