#pragma once
#include "D2SkillObject.h"
class CD2Teleport :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2Teleport();
    CD2Teleport(const CD2Teleport& com);
    virtual ~CD2Teleport();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2Teleport* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;

private:
    class CColliderBox2D* mCollider;
    bool mbIsCollide;
    bool mbStart;
};

