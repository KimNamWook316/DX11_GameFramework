#pragma once

#include "Component/ObjectComponent.h"

class CD2Effect :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2Effect();
    CD2Effect(const CD2Effect& com);
    virtual ~CD2Effect();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2Effect* Clone() override;

public:
    void OnAnimEnd();
};

