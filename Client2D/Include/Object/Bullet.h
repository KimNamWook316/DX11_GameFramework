#pragma once

#include "GameObject\GameObject.h"
#include "Component/ColliderCircle.h"

class CBullet : public CGameObject
{
	friend class CScene;
public:
	CBullet();
	CBullet(const CBullet& bullet);
	virtual ~CBullet();

public:
	virtual bool Init();
	virtual	void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBullet* Clone();

public:
	void SetCollisionProfile(const std::string& name);

public:
	void OnCollisionEnter(const CollisionResult& result);
	void OnCollisionExit(const CollisionResult& result);

private:
	CSharedPtr<class CSpriteComponent> mSprite;
	CSharedPtr<CColliderCircle> mBody;
	float mDistance;
};

