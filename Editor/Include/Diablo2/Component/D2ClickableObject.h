#pragma once

#include "Component/ObjectComponent.h"

class CD2ClickableObject :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2ClickableObject();
    CD2ClickableObject(const CD2ClickableObject& com);
    virtual ~CD2ClickableObject();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2ClickableObject* Clone() override = 0;

public:
    virtual void OnClick() = 0;

protected:
    virtual void onMouseCollideEnter(const CollisionResult& result);
    virtual void onMouseCollideExit(const CollisionResult& result);

protected:
    class CColliderBox2D* mCollider;
    class CD2StateComponent* mPlayerState;
    class CD2ClickableUIComponent* mUI;
};

