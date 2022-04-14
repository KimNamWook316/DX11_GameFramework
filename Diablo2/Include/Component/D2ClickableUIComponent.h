#pragma once

#include "Component/WidgetComponent.h"

class CD2ClickableUIComponent :
    public CWidgetComponent
{
    friend class CGameObject;

protected:
    CD2ClickableUIComponent();
    CD2ClickableUIComponent(const CD2ClickableUIComponent& com);
    virtual ~CD2ClickableUIComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CD2ClickableUIComponent* Clone() override;
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

private:
    bool mbInit;
};

