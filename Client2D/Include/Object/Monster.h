#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/WidgetComponent.h"

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
    void AddHP(const int hp)
    {
        mHP += hp;
        if (mHP <= 0)
        {
            Destroy();
        }
    }

private:
    CSharedPtr<CSpriteComponent> mSprite;
    CSharedPtr<CColliderBox2D> mBody;
	CSharedPtr<CWidgetComponent> mSimpleHUDWidget;
    class CSimpleHUD* mSimpleHUD;

    int mHP;
};

