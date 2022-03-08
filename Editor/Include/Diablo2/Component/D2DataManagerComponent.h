#pragma once

#include "Component/ObjectComponent.h"

class CD2DataManagerComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2DataManagerComponent();
    CD2DataManagerComponent(const CD2DataManagerComponent& com) = delete;
    virtual ~CD2DataManagerComponent();
    
public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CObjectComponent* Clone() override;

    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;
};

