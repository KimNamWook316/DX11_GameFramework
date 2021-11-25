#pragma once
#include "GameObject\GameObject.h"
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

private:
	CSharedPtr<class CSpriteComponent> mSprite;
};

