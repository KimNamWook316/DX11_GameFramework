#pragma once

#include "ObjectComponent.h"
#include "../Resource/Shader/DissolveConstantBuffer.h"

class CDissolveComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CDissolveComponent();
    CDissolveComponent(CDissolveComponent& com);
    virtual ~CDissolveComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CDissolveComponent* Clone() override;

private:
    CDissolveConstantBuffer* mCBuffer;
};

