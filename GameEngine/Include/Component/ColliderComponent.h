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

public:
    void SetColiderType(eColliderType type)
    {
        meColliderType = type;
    }

protected:
    eColliderType meColliderType;
};

