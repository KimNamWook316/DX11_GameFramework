#pragma once

#include "Component/ObjectComponent.h"
#include "../D2Info.h"

class CD2SkillObject :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2SkillObject();
    CD2SkillObject(const CD2SkillObject& com);
    virtual ~CD2SkillObject();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;

public:
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

public:
    virtual void OnCollideEnter(const CollisionResult& result) = 0;

public:
    void SetInfo(const std::string& name);

    void SetInfo(const D2SkillInfo& info)
    {
        mInfo = info;
    }

    virtual void SetDir(const Vector2& dir)
    {
        mInfo.Dir = dir;
    }

    void SetSkillOwner(class CGameObject* obj)
    {
        mSkillOwner = obj;
    }

    void SetDamage(const float damage)
    {
        mInfo.Damage = damage;
    }

    void SetTargetObject(class CGameObject* obj)
    {
        mTargetObj = obj;
    }

    void SetTargetPos(const Vector3& pos)
    {
        mTargetPos = pos;
    }

    void SetSpeed(const float speed)
    {
        mInfo.Speed = speed;
    }

    void SetEditMode(bool bEdit)
    {
        mbEditMode = bEdit;
    }

public:
    float GetDamage() const
    {
        return mInfo.Damage;
    }

protected:
    class CGameObject* mSkillOwner;
    class CGameObject* mTargetObj;
    Vector3 mTargetPos;
    D2SkillInfo mInfo;
    CSharedPtr<class CSceneComponent> mRoot;
    bool mbEditMode;
};

