#pragma once

#include "ColliderComponent.h"

class CColliderBox2D :
    public CColliderComponent
{
    friend class CGameObject;

protected:
    CColliderBox2D();
    CColliderBox2D(const CColliderBox2D& com);
    virtual ~CColliderBox2D();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderBox2D* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

    virtual bool DoCollide(CColliderComponent* Dest);
    virtual bool DoCollideMouse(const Vector2& mousePos);

public:
    Box2DInfo GetInfo() const
    {
        return mInfo;
    }

public:
    void SetExtent(float width, float height)
    {
        mInfo.Length.x = width;
        mInfo.Length.y = height;
        SetWorldScale(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);
    }

    void SetWidth(const float width)
    {
        mInfo.Length.x = width;
        SetWorldScale(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);
    }

    void SetHeight(const float height)
    {
        mInfo.Length.y = height;
        SetWorldScale(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);
    }

private:
    Box2DInfo mInfo;
};

