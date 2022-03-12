#pragma once

#include "Component/ObjectComponent.h"

class CD2ObjectPoolComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CD2ObjectPoolComponent();
	CD2ObjectPoolComponent(const CD2ObjectPoolComponent& com);
	~CD2ObjectPoolComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CD2ObjectPoolComponent* Clone() override;
};

