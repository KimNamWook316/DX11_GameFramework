#pragma once

#include "D2ClickableObject.h"

class CD2PortalObject :
    public CD2ClickableObject
{
    friend class CGameObject;

protected:
    CD2PortalObject();
    CD2PortalObject(const CD2PortalObject& com);
    virtual ~CD2PortalObject();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2PortalObject* Clone() override;

public:
    virtual void OnClick() override;

private:
    void onAppearAnimEnd();

private:
    class CSpriteComponent* mSprite;
};

