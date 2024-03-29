#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/DissolveComponent.h"

class CMonster :
    public CGameObject
{
    friend class CScene;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();
    
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMonster* Clone();

public:
    void OnCollsionMouseCallBack(const CollisionResult& result);
    void OnCollsionExitMouseCallBack(const CollisionResult& result);

public:
	void OnCollisionEnter(const CollisionResult& result);
	void OnCollisionExit(const CollisionResult& result);

private:
    void dissolveEnd();

private:
    CSharedPtr<CSpriteComponent> mSprite;
    CSharedPtr<CColliderCircle> mBody;
    CSharedPtr<CDissolveComponent> mDissolve;
	CSharedPtr<CWidgetComponent> mSimpleHUDWidget;
    class CSimpleHUD* mSimpleHUD;
    int mHP;
};

