#pragma once

#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"

class CPlayerChild : public CGameObject 
{
public:
	CPlayerChild();
	CPlayerChild(const CPlayerChild& obj);
	virtual ~CPlayerChild();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CGameObject* Clone();

private:
	CSharedPtr<CSpriteComponent> mSprite;
	float mLookAngle;
	bool mbIsCaptured;
};

