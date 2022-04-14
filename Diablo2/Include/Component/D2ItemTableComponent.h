#pragma once

#include "Component\ObjectComponent.h"

class CD2ItemTableComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2ItemTableComponent();
    CD2ItemTableComponent(const CD2ItemTableComponent& com);
    virtual ~CD2ItemTableComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2ItemTableComponent* Clone() override;

private:
    bool mbInit;
};

