#pragma once

#include "ColliderComponent.h"

class CColliderCircle :
    public CColliderComponent
{
    friend class CGameObject;

protected:
    CColliderCircle();
    CColliderCircle(const CColliderCircle& com);
    virtual ~CColliderCircle();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderCircle* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

    virtual bool DoCollide(CColliderComponent* Dest) override;
    virtual bool DoCollideMouse(const Vector2& mousePos) override;

public:
    CircleInfo GetInfo() const
    {
        return mInfo;
    }

public:
    void SetInfo(const Vector2& center, float radius)
    {
        mInfo.Center = center;
        mInfo.Radius = radius;

        SetWorldScale(mInfo.Radius * 2.f, mInfo.Radius * 2.f, 1.f);
    }

protected:
    CircleInfo mInfo;
};

