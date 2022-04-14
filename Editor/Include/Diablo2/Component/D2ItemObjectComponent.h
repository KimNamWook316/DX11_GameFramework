#pragma once

#include "D2ClickableObject.h"

class CD2ItemObjectComponent :
    public CD2ClickableObject
{
    friend class CGameObject;

protected:
    CD2ItemObjectComponent();
    CD2ItemObjectComponent(const CD2ItemObjectComponent& com);
    virtual ~CD2ItemObjectComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2ItemObjectComponent* Clone() override;

public:
    void OnClick() override;

public:
    void SetItem(class CD2Item* item);

private:
    CSharedPtr<class CD2Item> mItem;
    class CSpriteComponent* mSprite;
};

