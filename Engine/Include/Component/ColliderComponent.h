#pragma once

#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CColliderComponent();
    CColliderComponent(const CColliderComponent& com);
    virtual ~CColliderComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void CheckCollision() override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderComponent* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

public:
    eColliderType GetColliderType() const
    {
        return meColliderType;
    }

    Vector3 GetMinPos() const
    {
        return mMinPos;
    }

    Vector3 GetMaxPos() const
    {
        return mMaxPos;
    }

public:
    void SetColiderType(eColliderType type)
    {
        meColliderType = type;
    }

    void SetOffset(const Vector3& offset)
    {
        mOffset = offset;
    }

    void SetOffset(const float x, const float y, const float z)
    {
        mOffset = Vector3(x, y, z);
    }

protected:
    eColliderType meColliderType;
    Vector3 mOffset;
    Vector3 mMinPos;
    Vector3 mMaxPos;
};

