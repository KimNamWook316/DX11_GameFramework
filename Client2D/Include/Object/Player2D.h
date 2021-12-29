#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

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
	void move(float deltaTime);
	void moveUp(float deltaTime);
	void moveDown(float deltaTime);
	void rotationZInv(float deltaTime);
	void rotationZ(float deltaTime);
	void attack();
	
	void playAttackAnim(float deltaTime);

private:
	CSharedPtr<CSpriteComponent> mSprite;
	CSharedPtr<CColliderBox2D> mBody;

	float mOpacity;
	float mSpeed;
	bool mbIsMoving;
	Vector3 mMovePosition;
	Vector3 mDirection;
};

