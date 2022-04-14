#pragma once

#include "Component/ObjectComponent.h"

class CD2DungeonManagerComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2DungeonManagerComponent();
    CD2DungeonManagerComponent(const CD2DungeonManagerComponent& com);
    virtual ~CD2DungeonManagerComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2DungeonManagerComponent* Clone() override;

private:
	bool mbInit;
};

