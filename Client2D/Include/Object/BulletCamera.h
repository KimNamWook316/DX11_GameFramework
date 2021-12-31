#pragma once

#include "GameObject/GameObject.h"

class CBulletCamera :
    public CGameObject
{
	friend class CScene;
public:
	CBulletCamera();
	CBulletCamera(const CBulletCamera& bullet);
	virtual ~CBulletCamera();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual	void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual CBulletCamera* Clone();

private:
	void OnCollisionCallBack(const CollisionResult& result);

private:
	CSharedPtr<class CSpriteComponent> mSprite;
	CSharedPtr<class CColliderBox2D> mBody;
	CSharedPtr<class CCameraComponent> mCamera;
};

