#pragma once

#include "Component/ObjectComponent.h"

class CD2UIManagerComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2UIManagerComponent();
    CD2UIManagerComponent(const CD2UIManagerComponent& com);
    virtual ~CD2UIManagerComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2UIManagerComponent* Clone() override;
};

