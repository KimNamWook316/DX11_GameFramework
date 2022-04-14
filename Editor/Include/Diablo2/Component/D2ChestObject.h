#pragma once

#include "D2ClickableObject.h"

class CD2ChestObject :
    public CD2ClickableObject
{
    friend class CGameObject;

protected:
    CD2ChestObject();
    CD2ChestObject(const CD2ChestObject& com);
    virtual ~CD2ChestObject();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2ChestObject* Clone() override;

public:
    virtual void OnClick() override;

private:
    bool mbOpen;
    class CSpriteComponent* mSprite;
    class CDissolveComponent* mDissolve;
    CSharedPtr<class CD2Item> mItem;
};

