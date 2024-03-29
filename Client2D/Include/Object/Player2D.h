#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"

class CPlayer2D : public CGameObject
{
public:
	CPlayer2D();
	CPlayer2D(const CPlayer2D& obj);
	virtual ~CPlayer2D();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CGameObject* Clone();

private:
	void moveUp(float deltaTime);
	void moveDown(float deltaTime);
	void rotationZInv(float deltaTime);
	void rotationZ(float deltaTime);
	void attack(float deltaTime);
	void attack1(float deltaTime);
	void skill1(float deltaTime);

private:
	CSharedPtr<CSpriteComponent>    mSprite;
	CSharedPtr<CSpriteComponent>    mChildLeftSprite;
	CSharedPtr<CSpriteComponent>    mChildRightSprite;
	CSharedPtr<CSceneComponent>     mChildLeftMuzzle;
	CSharedPtr<CSceneComponent>     mChildRightMuzzle;
	CSharedPtr<CSceneComponent>     mChildRoot;
	CSharedPtr<CSceneComponent>     mMuzzle;

	CSharedPtr<CSpriteComponent>    mChild1Sprite;
	CSharedPtr<CSpriteComponent>    mChild2Sprite;
	CSharedPtr<CSpriteComponent>    mChild3Sprite;
	CSharedPtr<CSpriteComponent>    mChild4Sprite;

	CSharedPtr<CColliderBox2D>       mBody;
	CSharedPtr<CCameraComponent>     mCamera;

	CSharedPtr<CWidgetComponent>     mSimpleHUDWidget;

	float mOpacity;
};

