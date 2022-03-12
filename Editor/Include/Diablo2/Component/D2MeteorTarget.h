#pragma once
#include "D2SkillObject.h"
class CD2MeteorTarget :
    public CD2SkillObject
{
    friend class CGameObject;

protected:
    CD2MeteorTarget();
    CD2MeteorTarget(const CD2MeteorTarget& com);
    virtual ~CD2MeteorTarget();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Start() override;
    virtual CD2MeteorTarget* Clone() override;
    virtual void OnCollideEnter(const CollisionResult& result) override;

protected:
    float mTimer;
    bool mbFirstFrame;
};

